/*
 * GraphLCD plugin for the Video Disk Recorder
 *
 * display.h - display class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2001-2004 Carsten Siebholz <c.siebholz AT t-online.de>
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#ifndef GRAPHLCD_DISPLAY_H
#define GRAPHLCD_DISPLAY_H

#include <stdint.h>

#include <string>
#include <vector>

#include <glcdgraphics/bitmap.h>

#include "global.h"
#include "setup.h"
#include "state.h"

#include <vdr/thread.h>


enum eThreadState
{
    StateNormal,
    StateReplay,
    StateMenu
};

// Display update Thread
class cGraphLCDDisplay : public cThread
{
public:
    cGraphLCDDisplay(void);
    ~cGraphLCDDisplay(void);

    int Init(GLCD::cDriver * Lcd, const char * CfgDir);
    void Tick(void);

    void Update();

protected:
    virtual void Action();

private:
    GLCD::cDriver * mLcd;

    bool mUpdate;
    uint64_t mUpdateAt;
    uint64_t mLastTimeMs;

    GLCD::cBitmap * bitmap;
    std::string cfgDir;

    eThreadState State;
    eThreadState LastState;

    cMutex mMutex;
    cGraphLCDState * mGraphLCDState;

    bool showVolume;

    void UpdateIn(uint64_t msec);
};

#endif
