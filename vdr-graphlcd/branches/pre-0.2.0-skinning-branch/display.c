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
#include <glcdskin/parser.h>

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
    mScreen(NULL),
    mSkin(NULL),
    mSkinConfig(NULL),
    mGraphLCDState(NULL)
{
    mUpdate = false;
    mUpdateAt = 0;
    mLastTimeMs = 0;

    mCfgPath = "";
    mSkinName = "";

    State = StateNormal;
    LastState = StateNormal;

    showVolume = false;
}

cGraphLCDDisplay::~cGraphLCDDisplay()
{
    Cancel(3);

    delete mGraphLCDState;
    delete mSkin;
    delete mSkinConfig;
    delete mScreen;
}

bool cGraphLCDDisplay::Initialise(GLCD::cDriver * Lcd, const std::string & CfgPath, const std::string & SkinName)
{
    std::string skinFileName;

    if (!Lcd)
        return false;
    mLcd = Lcd;
    mCfgPath = CfgPath;
    mSkinName = SkinName;

    mScreen = new GLCD::cBitmap(mLcd->Width(), mLcd->Height());
    if (!mScreen)
    {
        esyslog("graphlcd plugin: ERROR creating drawing bitmap\n");
        return false;
    }

    mSkinConfig = new cGraphLCDSkinConfig(mCfgPath, mSkinName);
    if (!mSkinConfig)
    {
        esyslog("graphlcd plugin: ERROR creating skin config\n");
        return false;
    }

    skinFileName = mSkinConfig->SkinPath() + "/" + mSkinName + ".skin";
    mSkin = GLCD::XmlParse(*mSkinConfig, mSkinName, skinFileName);
    mSkin->SetBaseSize(mScreen->Width(), mScreen->Height());

    mGraphLCDState = new cGraphLCDState(this);
    if (!mGraphLCDState)
        return false;

    mLcd->Refresh(true);
    mUpdate = true;

    Start();
    return true;
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

                        GLCD::cSkinDisplay * display = mSkin->Get(GLCD::cSkinDisplay::normal);
                        mScreen->Clear();
                        display->Render(mScreen);
                        mLcd->SetScreen(mScreen->Data(), mScreen->Width(), mScreen->Height(), mScreen->LineSize());
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
