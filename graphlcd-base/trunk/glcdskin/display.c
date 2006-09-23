/*
 * GraphLCD skin library
 *
 * display.c  -  display class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#include "display.h"

namespace GLCD
{

static const std::string DisplayNames[] =
  { "normal", "volume", "message", "replay", "menu" };

cSkinDisplay::cSkinDisplay(cSkin * parent)
: skin(parent),
  type((eType)__COUNT_DISPLAY__)
{
}

bool cSkinDisplay::ParseType(const std::string & Text)
{
  for (int i = 0; i < (int) __COUNT_DISPLAY__; ++i)
  {
    if (DisplayNames[i].length() > 0 && DisplayNames[i] == Text)
    {
      type = (eType) i;
      return true;
    }
  }
  return false;
}

const std::string & cSkinDisplay::GetType(eType Type)
{
  return DisplayNames[Type];
}

void cSkinDisplay::Render(cBitmap * screen)
{
  for (uint32_t i = 0; i < NumObjects(); ++i)
    GetObject(i)->Render(screen);
}


cSkinDisplays::cSkinDisplays(void)
{
}

cSkinDisplays::~cSkinDisplays()
{
  iterator it = begin();
  while (it != end())
  {
    delete (*it).second;
    it++;
  }
}

} // end of namespace

