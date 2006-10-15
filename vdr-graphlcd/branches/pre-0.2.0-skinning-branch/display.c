/*
 * GraphLCD plugin for the Video Disk Recorder
 *
 * display.c - display class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2001-2004 Carsten Siebholz <c.siebholz AT t-online.de>
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#include <stdlib.h>

#include <algorithm>

#include <glcddrivers/config.h>
#include <glcddrivers/drivers.h>

#include "display.h"
#include "global.h"
#include "i18n.h"
#include "setup.h"
#include "state.h"
#include "strfct.h"

#include <vdr/tools.h>
#include <vdr/menu.h>

cGraphLCDDisplay::cGraphLCDDisplay()
:   cThread("glcd_display"),
    mLcd(NULL),
    bitmap(NULL),
    mGraphLCDState(NULL)
{
    mUpdate = false;
    mUpdateAt = 0;
    mLastTimeMs = 0;

    cfgDir = "";

    State = StateNormal;
    LastState = StateNormal;

    showVolume = false;
}

cGraphLCDDisplay::~cGraphLCDDisplay()
{
    Cancel(3);

    delete mGraphLCDState;
    delete bitmap;
}

int cGraphLCDDisplay::Init(GLCD::cDriver * Lcd, const char * CfgDir)
{
    if (!Lcd || !CfgDir)
        return 2;
    mLcd = Lcd;
    cfgDir = CfgDir;

    bitmap = new GLCD::cBitmap(mLcd->Width(), mLcd->Height());
    if (!bitmap)
    {
        esyslog("graphlcd plugin: ERROR creating drawing bitmap\n");
        return 1;
    }

    mGraphLCDState = new cGraphLCDState(this);
    if (!mGraphLCDState)
        return 1;

    mLcd->Refresh(true);
    mUpdate = true;

    Start();
    return 0;
}

void cGraphLCDDisplay::Tick(void)
{
    if (mGraphLCDState)
        mGraphLCDState->Tick();
}

void cGraphLCDDisplay::Action(void)
{
    while (Running())
    {
        if (GraphLCDSetup.PluginActive)
        {
            uint64_t currTimeMs = cTimeMs::Now();

            if (mUpdateAt != 0)
            {
                // timed Update enabled
                if (currTimeMs > mUpdateAt)
                {
                    mUpdateAt = 0;
                    mUpdate = true;
                }
            }
            if (GraphLCDSetup.ShowVolume && !mUpdate && showVolume)
            {
                if (currTimeMs - mGraphLCDState->GetVolumeState().lastChange > 2000)
                {
                    mUpdate = true;
                    showVolume = false;
                }
            }

            switch (State)
            {
                case StateNormal:
                    // update Display every minute or due to an update
                    if (currTimeMs/60000 != mLastTimeMs/60000 || mUpdate)
                    {
                        mUpdateAt = 0;
                        mUpdate = false;

                        bitmap->Clear();
                        mLcd->SetScreen(bitmap->Data(), bitmap->Width(), bitmap->Height(), bitmap->LineSize());
                        mLcd->Refresh(false);
                        mLastTimeMs = currTimeMs;
                    }
                    else
                    {
                        cCondWait::SleepMs(100);
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            cCondWait::SleepMs(100);
        }
    }
}

void cGraphLCDDisplay::Update()
{
    mUpdate = true;
}

void cGraphLCDDisplay::UpdateIn(uint64_t msec)
{
    if (msec == 0)
    {
        mUpdateAt = 0;
    }
    else
    {
        mUpdateAt = cTimeMs::Now() + msec;
    }
}
