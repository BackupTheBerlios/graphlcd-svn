/*
 * GraphLCD plugin for the Video Disk Recorder
 *
 * skinconfig.c - skin config class that implements all the callbacks
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#include <glcdskin/config.h>
#include <glcdskin/type.h>

#include "skinconfig.h"

cGraphLCDSkinConfig::cGraphLCDSkinConfig(const std::string & CfgPath, const std::string & SkinName)
{
    mSkinPath = CfgPath + "/skins/" + SkinName;
}

cGraphLCDSkinConfig::~cGraphLCDSkinConfig()
{
}

std::string cGraphLCDSkinConfig::SkinPath(void) const
{
    return mSkinPath;
}

std::string cGraphLCDSkinConfig::CharSet(void) const
{
    return "iso-8859-15";
}

std::string cGraphLCDSkinConfig::Translate(const std::string & Text) const
{
    return Text;
}

GLCD::cType cGraphLCDSkinConfig::GetToken(const GLCD::tSkinToken & Token) const
{
    return "";
}
