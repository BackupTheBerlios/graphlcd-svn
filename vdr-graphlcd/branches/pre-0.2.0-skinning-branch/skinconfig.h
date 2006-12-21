/*
 * GraphLCD plugin for the Video Disk Recorder
 *
 * skinconfig.h - skin config class that implements all the callbacks
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#ifndef _GRAPHLCD_SKINCONFIG_H_
#define _GRAPHLCD_SKINCONFIG_H_

#include "alias.h"

class cGraphLCDSkinConfig : public GLCD::cSkinConfig
{
private:
    const cGraphLCDDisplay * mDisplay;
    std::string mConfigPath;
    std::string mSkinPath;
    cGraphLCDState * mState;
    cChannelAliasList mAliasList;
public:
    cGraphLCDSkinConfig(const cGraphLCDDisplay * Display, const std::string & CfgPath, const std::string & SkinsPath, const std::string & SkinName, cGraphLCDState * State);
    virtual ~cGraphLCDSkinConfig();

    virtual std::string SkinPath(void) const;
    virtual std::string CharSet(void) const;
    virtual std::string Translate(const std::string & Text) const;
    virtual GLCD::cType GetToken(const GLCD::tSkinToken & Token) const;
    virtual int GetTokenId(const std::string & Name) const;
};

#endif
