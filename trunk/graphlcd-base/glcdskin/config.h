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
class cFont;
struct tSkinToken;

class cSkinConfig
{
public:
    virtual ~cSkinConfig(void);
    virtual std::string SkinPath(void);
    virtual std::string FontPath(void);
    virtual std::string CharSet(void);
    virtual std::string Translate(const std::string & Text);
    virtual cType GetToken(const tSkinToken & Token);
    virtual int GetTokenId(const std::string & Name);
    virtual int GetTabPosition(int Index, int MaxWidth, const cFont & Font);
};

} // end of namespace

#endif
