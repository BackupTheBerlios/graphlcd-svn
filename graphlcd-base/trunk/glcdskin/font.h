/*
 * GraphLCD skin library
 *
 * font.h  -  font class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_FONT_H_
#define _GLCDSKIN_FONT_H_

#include <string>
#include <vector>

#include <glcdgraphics/font.h>

#include "object.h"

namespace GLCD
{

class cSkin;

class cSkinFont
{
    friend bool StartElem(const std::string &name, std::map<std::string,std::string> &attrs);
    friend bool EndElem(const std::string &name);

public:
    enum eType
    {
        ftFNT,
        ftFT2
    };

private:
    cSkin * skin;
    std::string id;
    eType type;
    std::string file;
    int size;
    cFont font;

public:
    cSkinFont(cSkin * Parent);

    bool ParseUrl(const std::string & Text);

    cSkin * Skin(void) const { return skin; }
    const std::string & Id(void) const { return id; }
    const cFont * Font(void) const { return &font; }
};

class cSkinFonts: public std::vector<cSkinFont *>
{
public:
    cSkinFonts(void);
    ~cSkinFonts(void);
};

} // end of namespace

#endif
