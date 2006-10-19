/*
 * GraphLCD skin library
 *
 * skin.h  -  skin class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_SKIN_H_
#define _GLCDSKIN_SKIN_H_

#include <map>
#include <string>

#include "display.h"
#include "font.h"
#include "type.h"
#include "string.h"
#include "cache.h"
#include "config.h"


namespace GLCD
{

class cSkin
{
    friend bool StartElem(const std::string & name, std::map<std::string,std::string> & attrs);
    friend bool EndElem(const std::string & name);

private:
    const cSkinConfig & config;
    std::string name;
    std::string title;
    std::string version;
    tSize baseSize;

    cSkinFonts fonts;
    cSkinDisplays displays;
    cImageCache * mImageCache;

public:
    cSkin(const cSkinConfig & Config, const std::string & Name);
    ~cSkin(void);

    void SetBaseSize(int width, int height);

    cSkinFont * GetFont(const std::string & id);
    cSkinDisplay * Get(cSkinDisplay::eType Type);

    const cSkinConfig & Config(void) const { return config; }
    const std::string & Name(void) const { return name; }
    const std::string & Title(void) const { return title; }
    const std::string & Version(void) const { return version; }
    const tSize & BaseSize(void) const { return baseSize; }

    cImageCache * ImageCache(void) { return mImageCache; }
};

inline cSkinFont * cSkin::GetFont(const std::string & id)
{
    cSkinFonts::iterator it = fonts.begin();
    while (it != fonts.end())
    {
        if ((*it)->Id() == id)
            return (*it);
        it++;
    }
    return NULL;
}

inline cSkinDisplay * cSkin::Get(cSkinDisplay::eType Type)
{
    if (displays.find(Type) != displays.end())
        return displays[Type];
    return NULL;
}

} // end of namespace

#endif
