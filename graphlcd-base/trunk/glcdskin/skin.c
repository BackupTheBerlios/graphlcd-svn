/*
 * GraphLCD skin library
 *
 * skin.c  -  skin class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#include "skin.h"

namespace GLCD
{

cSkin::cSkin(cSkinConfig & Config, const std::string & Name)
:   config(Config),
    name(Name)
{
    mImageCache = new cImageCache(this, 100);
}

cSkin::~cSkin(void)
{
    delete mImageCache;
}

void cSkin::SetBaseSize(int width, int height)
{
    baseSize.w = width;
    baseSize.h = height;
}

} // end of namespace
