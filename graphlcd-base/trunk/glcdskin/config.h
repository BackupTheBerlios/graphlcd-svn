/*
 * GraphLCD skin library
 *
 * config.h  -  config class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_CONFIG_H_
#define _GLCDSKIN_CONFIG_H_

#include <string>


namespace GLCD
{

class cType;
struct tSkinToken;

class cSkinConfig
{
public:
    virtual std::string SkinPath(void) const;
    virtual std::string CharSet(void) const;
    virtual std::string Translate(const std::string & Text) const;
    virtual cType GetToken(const tSkinToken & Token) const;
    virtual int GetTokenId(const std::string & Name) const;
};

} // end of namespace

#endif
