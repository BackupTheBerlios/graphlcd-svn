/*
 * GraphLCD skin library
 *
 * display.h  -  display class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_DISPLAY_H_
#define _GLCDSKIN_DISPLAY_H_

#include <string>
#include <map>

#include "object.h"

namespace GLCD
{

class cSkin;

class cSkinDisplay
{
    friend bool StartElem(const std::string &name, std::map<std::string,std::string> &attrs);
    friend bool EndElem(const std::string &name);

public:
    enum eType
    {
        normal,
        volume,
        message,
        replay,
        menu,
#define __COUNT_DISPLAY__ (menu + 1)
    };

private:
    cSkin * skin;
    eType type;
    cSkinObjects objects;

public:
    cSkinDisplay(cSkin * Parent);

    static const std::string &GetType(eType Type);
    bool ParseType(const std::string &Text);

    cSkin * Skin(void) const { return skin; }
    eType Type(void) const { return type; }

    uint32_t NumObjects(void) const { return objects.size(); }
    cSkinObject * GetObject(uint32_t n) const { return objects[n]; }

    void Render(cBitmap * screen);
};

class cSkinDisplays: public std::map<cSkinDisplay::eType, cSkinDisplay *>
{
public:
    cSkinDisplays(void);
    ~cSkinDisplays(void);
};

} // end of namespace

#endif
