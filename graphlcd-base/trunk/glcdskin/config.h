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

typedef std::string (*tSkinPathFunc)(void);
typedef std::string (*tCharSetFunc)(void);
typedef std::string (*tTranslateFunc)(const std::string &);
typedef cType (*tGetTokenFunc)(const tSkinToken &);

class cSkinConfig
{
private:
    tSkinPathFunc skinPathFunc;
    tCharSetFunc charSetFunc;
    tTranslateFunc translateFunc;
    tGetTokenFunc getTokenFunc;
public:
    cSkinConfig(void);
    cSkinConfig(const cSkinConfig & Config);

    std::string SkinPath(void) const;
    std::string CharSet(void) const;
    std::string Translate(const std::string & Text) const;
    cType GetToken(const tSkinToken & Token) const;

    void SetSkinPathFunc(tSkinPathFunc Function);
    void SetCharSetFunc(tCharSetFunc Function);
    void SetTranslateFunc(tTranslateFunc Function);
    void SetGetTokenFunc(tGetTokenFunc Function);
};

} // end of namespace

#endif
