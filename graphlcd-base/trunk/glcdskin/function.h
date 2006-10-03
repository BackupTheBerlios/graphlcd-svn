/*
 * GraphLCD skin library
 *
 * function.h  -  skin functions class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_FUNCTION_H_
#define _GLCDSKIN_FUNCTION_H_

#include <stdint.h>

#include <string>

#include "type.h"
#include "string.h"

namespace GLCD
{

#define STRING    0x01000000
#define NUMBER    0x02000000
#define INTERNAL  0x04000000

#define MAXPARAMETERS 512

class cSkinObject;
class cSkin;

class cSkinFunction
{
public:
    enum eType
    {
        undefined_function,

        string = STRING,
        number = NUMBER,

        fun_not = INTERNAL,
        fun_and,
        fun_or,
        fun_eq,
        fun_gt,
        fun_lt,
        fun_ge,
        fun_le,
        fun_ne,
        fun_file,
        fun_trans
    };

private:
    cSkinObject   * mObject;
    cSkin         * mSkin;
    eType           mType;
    cSkinString     mString;
    int             mNumber;
    cSkinFunction * mParams[MAXPARAMETERS];
    uint32_t        mNumParams;

protected:
    cType FunFile  (const cType &Param) const;
    cType FunPlugin(const cType &Param) const;

public:
    cSkinFunction(cSkinObject *Parent);
    cSkinFunction(const cSkinString &String);
    cSkinFunction(const cSkinFunction &Src);
    ~cSkinFunction();

    bool Parse(const std::string &Text);
    cType Evaluate(void) const;

    void SetListIndex(uint32_t Index, int Tab);
};

inline void cSkinFunction::SetListIndex(uint32_t Index, int Tab)
{
    mString.SetListIndex(Index, Tab);
    for (uint32_t i = 0; i < mNumParams; i++)
        mParams[i]->SetListIndex(Index, Tab);
}

} // end of namespace

#endif
