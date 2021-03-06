/**
 *  GraphLCD plugin for the Video Disk Recorder
 *
 *  status.c  -  status monitor class
 *
 *  (c) 2001-2004 Carsten Siebholz <c.siebholz AT t-online de>
 **/

#include <ctype.h>

#include <algorithm>

#include "display.h"
#include "state.h"
#include "strfct.h"

#include <vdr/eitscan.h>
#include <vdr/i18n.h>


cGraphLCDState::cGraphLCDState(cGraphLCDDisplay * Display)
:   mDisplay(Display),
    first(true),
    tickUsed(false)
{
    mChannel.id = tChannelID::InvalidID;
    mChannel.number = 0;
    mChannel.name = "";
    mChannel.shortName = "";
    mChannel.provider = "";
    mChannel.portal = "";
    mChannel.source = "";
    mChannel.hasTeletext = false;
    mChannel.hasMultiLanguage = false;
    mChannel.hasDolby = false;
    mChannel.isEncrypted = false;
    mChannel.isRadio = false;

    mPresent.valid = false;
    mPresent.startTime = 0;
    mPresent.vpsTime = 0;
    mPresent.duration = 0;
    mPresent.title = "";
    mPresent.shortText = "";
    mPresent.description = "";

    mFollowing.valid = false;
    mFollowing.startTime = 0;
    mFollowing.vpsTime = 0;
    mFollowing.duration = 0;
    mFollowing.title = "";
    mFollowing.shortText = "";
    mFollowing.description = "";

    mReplay.name = "";
    mReplay.loopmode = "";
    mReplay.control = NULL;
    mReplay.mode = eReplayNormal;
    mReplay.current = 0;
    mReplay.total = 0;

    mOsd.currentItem = "";
    mOsd.title = "";
    mOsd.redButton = "";
    mOsd.greenButton = "";
    mOsd.yellowButton = "";
    mOsd.blueButton = "";
    mOsd.message = "";
    mOsd.textItem = "";
    mOsd.currentItemIndex = -1;

    mVolume.value = -1;
    mVolume.lastChange = 0;

    SetChannel(cDevice::CurrentChannel());
}

cGraphLCDState::~cGraphLCDState()
{
}

void cGraphLCDState::ChannelSwitch(const cDevice * Device, int ChannelNumber)
{
    //printf("graphlcd plugin: cGraphLCDState::ChannelSwitch %d %d\n", Device->CardIndex(), ChannelNumber);
    if (GraphLCDSetup.PluginActive)
    {
        if (ChannelNumber > 0 && Device->IsPrimaryDevice() && !EITScanner.UsesDevice(Device))
        {
            if (ChannelNumber == cDevice::CurrentChannel())
            {
                SetChannel(ChannelNumber);
            }
        }
    }
}

void cGraphLCDState::Recording(const cDevice * Device, const char * Name, const char *FileName, bool On)
{
    //printf("graphlcd plugin: cGraphLCDState::Recording %d %s\n", Device->CardIndex(), Name);
    if (GraphLCDSetup.PluginActive)
    {
        std::vector <tRecording>::iterator it;

        mutex.Lock();
        it = mRecordings.begin();
        while (it != mRecordings.end())
        {
            if (it->deviceNumber == Device->DeviceNumber()
                && it->fileName == FileName)
            {
                break;
            }
            it++;
        }

        if (On)
        {
            if (it == mRecordings.end())
            {
                tRecording rec;

                rec.deviceNumber = Device->DeviceNumber();
                rec.name = Name;
                rec.fileName = FileName;
                mRecordings.push_back(rec);
            }
        }
        else
        {
            if (it != mRecordings.end())
                mRecordings.erase(it);
        }
        mutex.Unlock();

        mDisplay->Update();
    }
}

void cGraphLCDState::Replaying(const cControl * Control, const char * Name, const char *FileName, bool On)
{
    //printf("graphlcd plugin: cGraphLCDState::Replaying %s\n", Name);
    if (GraphLCDSetup.PluginActive)
    {
        if (On)
        {
            mutex.Lock();
            mReplay.control = (cControl *) Control;
            mReplay.mode = eReplayNormal;
            mReplay.name = "";
            mReplay.loopmode = "";
            if (Name && !isempty(Name))
            {
                if (GraphLCDSetup.IdentifyReplayType)
                {
                    int slen = strlen(Name);
                    bool bFound = false;
                    ///////////////////////////////////////////////////////////////////////
                    //Looking for mp3-Plugin Replay : [LS] (444/666) title
                    //
                    if (slen > 6 &&
                        *(Name+0)=='[' &&
                        *(Name+3)==']' &&
                        *(Name+5)=='(')
                    {
                        unsigned int i;
                        for (i=6; *(Name + i) != '\0'; ++i) //search for [xx] (xxxx) title
                        {
                            if (*(Name+i)==' ' && *(Name+i-1)==')')
                            {
                                bFound = true;
                                break;
                            }
                        }
                        if (bFound) //found MP3-Plugin replaymessage
                        {
                            unsigned int j;
                            // get loopmode
                            mReplay.loopmode = Name;
                            mReplay.loopmode = mReplay.loopmode.substr (0, 5);
                            if (mReplay.loopmode[2] == '.')
                                mReplay.loopmode.erase (2, 1);
                            if (mReplay.loopmode[1] == '.')
                                mReplay.loopmode.erase (1, 1);
                            if (mReplay.loopmode[1] == ']')
                                mReplay.loopmode = "";
                            //printf ("loopmode=<%s>\n", mReplay.loopmode.c_str ());
                            for (j=0;*(Name+i+j) != '\0';++j) //trim name
                            {
                                if (*(Name+i+j)!=' ')
                                    break;
                            }

                            if (strlen(Name+i+j) > 0)
                            { //if name isn't empty, then copy
                                mReplay.name = Name + i + j;
                            }
                            else
                            { //if Name empty, set fallback title
                                mReplay.name = tr("Unknown title");
                            }
                            mReplay.mode = eReplayMusic;
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////
                    //Looking for DVD-Plugin Replay : 1/8 4/28,  de 2/5 ac3, no 0/7,  16:9, VOLUMENAME
                    //cDvdPlayerControl::GerDisplayHeaderLine
                    //                         titleinfo, audiolang,  spulang, aspect, title
                    if (!bFound)
                    {
                        if (slen>7)
                        {
                            unsigned int i,n;
                            for (n=0,i=0;*(Name+i) != '\0';++i)
                            { //search volumelabel after 4*", " => xxx, xxx, xxx, xxx, title
                                if (*(Name+i)==' ' && *(Name+i-1)==',')
                                {
                                    if (++n == 4)
                                    {
                                        bFound = true;
                                        break;
                                    }
                                }
                            }
                            if (bFound) //found DVD replaymessage
                            {
                                unsigned int j;bool b;
                                for (j=0;*(Name+i+j) != '\0';++j) //trim name
                                {
                                    if (*(Name+i+j)!=' ')
                                        break;
                                }

                                if (strlen(Name+i+j) > 0)
                                { // if name isn't empty, then copy
                                    mReplay.name = Name + i + j;
                                    // replace all '_' with ' '
                                    replace(mReplay.name.begin(), mReplay.name.end(), '_', ' ');
                                    for (j = 0, b = true; j < mReplay.name.length(); ++j)
                                    {
                                        // KAPITALIZE -> Kaptialize
                                        if (mReplay.name[j] == ' ')
                                            b = true;
                                        else if (b)
                                            b = false;
                                        else mReplay.name[j] = tolower(mReplay.name[j]);
                                    }
                                }
                                else
                                { //if Name empty, set fallback title
                                    mReplay.name = tr("Unknown title");
                                }
                                mReplay.mode = eReplayDVD;
                            }
                        }
                    }
                    if (!bFound)
                    {
                        int i;
                        for (i=slen-1;i>0;--i)
                        { //Reversesearch last Subtitle
                            // - filename contains '~' => subdirectory
                            // or filename contains '/' => subdirectory
                            switch (*(Name+i))
                            {
                                case '/':
                                {
                                    // look for file extentsion like .xxx or .xxxx
                                    if (slen>5 && ((*(Name+slen-4) == '.') || (*(Name+slen-5) == '.')))
                                    {
                                        mReplay.mode = eReplayFile;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                case '~':
                                {
                                    mReplay.name = Name + i + 1;
                                    bFound = true;
                                    i = 0;
                                }
                                default:
                                    break;
                            }
                        }
                    }

                    if (0 == strncmp(Name,"[image] ",8))
                    {
                        if (mReplay.mode != eReplayFile) //if'nt already Name stripped-down as filename
                            mReplay.name = Name + 8;
                        mReplay.mode = eReplayImage;
                        bFound = true;
                    }
                    else if (0 == strncmp(Name,"[audiocd] ",10))
                    {
                        mReplay.name = Name + 10;
                        mReplay.mode = eReplayAudioCD;
                        bFound = true;
                    }
                    if (!bFound || !GraphLCDSetup.ModifyReplayString)
                    {
                        mReplay.name = Name;
                    }
                }
                else
                {
                    mReplay.name = Name;
                }
            }
            mutex.Unlock();
        }
        else
        {
            mutex.Lock();
            mReplay.control = NULL;
            mutex.Unlock();
            SetChannel(mChannel.number);
        }
        mDisplay->Replaying(On);
    }
}

void cGraphLCDState::SetVolume(int Volume, bool Absolute)
{
    //printf("graphlcd plugin: cGraphLCDState::SetVolume %d %d\n", Volume, Absolute);
    if (GraphLCDSetup.PluginActive)
    {
        mutex.Lock();

        if (!Absolute)
        {
            mVolume.value += Volume;
        }
        else
        {
            mVolume.value = Volume;
        }
        if (!first)
        {
            mVolume.lastChange = cTimeMs::Now();
            mutex.Unlock();
            mDisplay->Update();
        }
        else
        {
            // first time
            first = false;
            mutex.Unlock();
        }
    }
}

void cGraphLCDState::Tick()
{
    //printf("graphlcd plugin: cGraphLCDState::Tick\n");
    if (GraphLCDSetup.PluginActive)
    {
        mutex.Lock();

        tickUsed = true;

        if (mReplay.control)
        {
            mReplay.control->GetReplayMode(mReplay.play, mReplay.forward, mReplay.speed);
            if (mReplay.control->GetIndex(mReplay.current, mReplay.total, false))
            {
                mReplay.total = (mReplay.total == 0) ? 1 : mReplay.total;
            }
            else
            {
                mReplay.control = NULL;
            }
        }

        mutex.Unlock();
    }
}

void cGraphLCDState::OsdClear()
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdClear\n");
    if (GraphLCDSetup.PluginActive)
    {
        mutex.Lock();

        mOsd.title = "";
        mOsd.items.clear();
        mOsd.currentItem = "";
        mOsd.currentItemIndex = -1;
        mOsd.redButton = "";
        mOsd.greenButton = "";
        mOsd.yellowButton = "";
        mOsd.blueButton = "";
        mOsd.message = "";
        mOsd.textItem = "";

        mutex.Unlock();
        mDisplay->SetMenuClear();
    }
}

void cGraphLCDState::OsdTitle(const char * Title)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdTitle '%s'\n", Title);
    if (GraphLCDSetup.PluginActive)
    {
        mutex.Lock();

        mOsd.message = "";
        mOsd.title = "";
        if (Title)
        {
            mOsd.title = Title;
            // remove the time
            std::string::size_type pos = mOsd.title.find('\t');
            if (pos != std::string::npos)
                mOsd.title.resize(pos);
            mOsd.title = compactspace(mOsd.title);
        }

        mutex.Unlock();
        mDisplay->SetMenuTitle();
    }
}

void cGraphLCDState::OsdStatusMessage(const char * Message)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdStatusMessage '%s'\n", Message);
    if (GraphLCDSetup.PluginActive)
    {
        if (GraphLCDSetup.ShowMessages)
        {
            mutex.Lock();

            if (Message)
                mOsd.message = compactspace(Message);
            else
                mOsd.message = "";

            mutex.Unlock();
            mDisplay->Update();
        }
    }
}

void cGraphLCDState::OsdHelpKeys(const char * Red, const char * Green, const char * Yellow, const char * Blue)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdHelpKeys %s - %s - %s - %s\n", Red, Green, Yellow, Blue);
    if (GraphLCDSetup.PluginActive)
    {
        if (GraphLCDSetup.ShowColorButtons)
        {
            mutex.Lock();

            mOsd.redButton = "";
            mOsd.greenButton = "";
            mOsd.yellowButton = "";
            mOsd.blueButton = "";

            if (Red)
                mOsd.redButton = compactspace(Red);
            if (Green)
                mOsd.greenButton = compactspace(Green);
            if (Yellow)
                mOsd.yellowButton = compactspace(Yellow);
            if (Blue)
                mOsd.blueButton = compactspace(Blue);

            mutex.Unlock();
        }
    }
}

void cGraphLCDState::OsdItem(const char * Text, int Index)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdItem %s, %d\n", Text, Index);
    if (GraphLCDSetup.PluginActive)
    {
        if (GraphLCDSetup.ShowMenu)
        {
            mutex.Lock();

            mOsd.message = "";

            if (Text)
                mOsd.items.push_back(Text);

            mutex.Unlock();
            //if (Text)
            //    mDisplay->SetOsdItem(Text);
        }
    }
}

void cGraphLCDState::OsdCurrentItem(const char * Text)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdCurrentItem %s\n", Text);
    if (GraphLCDSetup.PluginActive)
    {
        if (GraphLCDSetup.ShowMenu)
        {
            mutex.Lock();

            mOsd.message = "";
            mOsd.currentItem = "";
            if (Text)
            {
                uint32_t i;

                mOsd.currentItem = Text;
                mOsd.currentItemIndex = -1;
                for (i = 0; i < mOsd.items.size(); i++)
                {
                    if (mOsd.items[i].compare(mOsd.currentItem) == 0)
                    {
                        mOsd.currentItemIndex = i;
                        break;
                    }
                }
                if (i == mOsd.items.size())
                {
                    // maybe this is a settings menu with edit items, so
                    // just one tab
                    std::string::size_type pos = mOsd.currentItem.find('\t');
                    if (pos != std::string::npos && pos == mOsd.currentItem.rfind('\t'))
                    {
                        for (i = 0; i < mOsd.items.size(); i++)
                        {
                            if (mOsd.items[i].compare(0, pos, mOsd.currentItem, 0, pos) == 0)
                            {
                                mOsd.items[i] = mOsd.currentItem;
                                mOsd.currentItemIndex = i;
                                break;
                            }
                        }
                    }
                }
            }
            mutex.Unlock();
            if (Text)
                mDisplay->SetMenuCurrent();
        }
    }
}

void cGraphLCDState::OsdTextItem(const char * Text, bool Scroll)
{
    //esyslog("graphlcd plugin: cGraphLCDState::OsdTextItem %s %d\n", Text, Scroll);
    if (GraphLCDSetup.PluginActive)
    {
        mutex.Lock();
        if (Text)
        {
            mOsd.textItem = trim(Text);
        }
        mutex.Unlock();
        //mDisplay->SetOsdTextItem(Text, Scroll);
    }
}


void cGraphLCDState::OsdChannel(const char * Text)
{
    //printf("graphlcd plugin: cGraphLCDState::OsdChannel %s\n", Text);
    if (GraphLCDSetup.PluginActive)
    {
        if (Text)
            mDisplay->Update();
    }
}

void cGraphLCDState::OsdProgramme(time_t PresentTime, const char * PresentTitle, const char * PresentSubtitle, time_t FollowingTime, const char * FollowingTitle, const char * FollowingSubtitle)
{
    //printf("graphlcd plugin: cGraphLCDState::OsdProgramme PT : %s\n", PresentTitle);
    //printf("graphlcd plugin: cGraphLCDState::OsdProgramme PST: %s\n", PresentSubtitle);
    //printf("graphlcd plugin: cGraphLCDState::OsdProgramme FT : %s\n", FollowingTitle);
    //printf("graphlcd plugin: cGraphLCDState::OsdProgramme FST: %s\n", FollowingSubtitle);
    if (GraphLCDSetup.PluginActive)
    {
        mDisplay->Update();
    }
}

void cGraphLCDState::SetChannel(int ChannelNumber)
{
    if (ChannelNumber == 0)
        return;

    mutex.Lock();

    mChannel.number = ChannelNumber;
    mPresent.startTime = 0;
    mFollowing.startTime = 0;

    mutex.Unlock();

    mDisplay->Update();
}

void cGraphLCDState::UpdateChannelInfo(void)
{
    if (mChannel.number == 0)
        return;

    mutex.Lock();

    cChannel * ch = Channels.GetByNumber(mChannel.number);
    if (ch)
    {
        mChannel.id = ch->GetChannelID();
        mChannel.name = ch->Name();
        mChannel.shortName = ch->ShortName(true);
        mChannel.provider = ch->Provider();
        mChannel.portal = ch->PortalName();
        mChannel.source = Sources.Get(ch->Source())->Description();
        mChannel.hasTeletext = ch->Tpid() != 0;
        mChannel.hasMultiLanguage = ch->Apid(1) != 0;
        mChannel.hasDolby = ch->Dpid(0) != 0;
        mChannel.isEncrypted = ch->Ca() != 0;
        mChannel.isRadio = (ch->Vpid() == 0) || (ch->Vpid() == 1) || (ch->Vpid() == 0x1FFF);
    }
    else
    {
        mChannel.id = tChannelID::InvalidID;
        mChannel.name = tr("*** Invalid Channel ***");
        mChannel.shortName = tr("*** Invalid Channel ***");
        mChannel.provider = "";
        mChannel.portal = "";
        mChannel.source = "";
        mChannel.hasTeletext = false;
        mChannel.hasMultiLanguage = false;
        mChannel.hasDolby = false;
        mChannel.isEncrypted = false;
        mChannel.isRadio = false;
    }

    mutex.Unlock();
}

void cGraphLCDState::UpdateEventInfo(void)
{
    mutex.Lock();
    const cEvent * present = NULL, * following = NULL;
    cSchedulesLock schedulesLock;

    // reset event data to empty values
    mPresent.valid = false;
    mPresent.startTime = 0;
    mPresent.vpsTime = 0;
    mPresent.duration = 0;
    mPresent.title = "";
    mPresent.shortText = "";
    mPresent.description = "";

    mFollowing.valid = false;
    mFollowing.startTime = 0;
    mFollowing.vpsTime = 0;
    mFollowing.duration = 0;
    mFollowing.title = "";
    mFollowing.shortText = "";
    mFollowing.description = "";

    const cSchedules * schedules = cSchedules::Schedules(schedulesLock);
    if (mChannel.id.Valid())
    {
        if (schedules)
        {
            const cSchedule * schedule = schedules->GetSchedule(mChannel.id);
            if (schedule)
            {
                if ((present = schedule->GetPresentEvent()) != NULL)
                {
                    mPresent.valid = true;
                    mPresent.startTime = present->StartTime();
                    mPresent.vpsTime = present->Vps();
                    mPresent.duration = present->Duration();
                    mPresent.title = "";
                    if (present->Title())
                        mPresent.title = present->Title();
                    mPresent.shortText = "";
                    if (present->ShortText())
                        mPresent.shortText = present->ShortText();
                    mPresent.description = "";
                    if (present->Description())
                        mPresent.description = present->Description();
                }
                if ((following = schedule->GetFollowingEvent()) != NULL)
                {
                    mFollowing.valid = true;
                    mFollowing.startTime = following->StartTime();
                    mFollowing.vpsTime = following->Vps();
                    mFollowing.duration = following->Duration();
                    mFollowing.title = "";
                    if (following->Title())
                        mFollowing.title = following->Title();
                    mFollowing.shortText = "";
                    if (following->ShortText())
                        mFollowing.shortText = following->ShortText();
                    mFollowing.description = "";
                    if (following->Description())
                        mFollowing.description = following->Description();
                }
            }
        }
    }
    mutex.Unlock();
}

void cGraphLCDState::UpdateReplayInfo(void)
{
    mutex.Lock();
    if (!tickUsed)
    {
        if (mReplay.control)
        {
            mReplay.control->GetReplayMode(mReplay.play, mReplay.forward, mReplay.speed);
            if (mReplay.control->GetIndex(mReplay.current, mReplay.total, false))
            {
                mReplay.total = (mReplay.total == 0) ? 1 : mReplay.total;
            }
            else
            {
                mReplay.control = NULL;
            }
        }
    }
    mutex.Unlock();
}

void cGraphLCDState::Update()
{
    UpdateChannelInfo();
    UpdateEventInfo();
    UpdateReplayInfo();
}

tChannel cGraphLCDState::GetChannelInfo()
{
    tChannel ret;

    mutex.Lock();
    ret = mChannel;
    mutex.Unlock();

    return ret;
}

tEvent cGraphLCDState::GetPresentEvent()
{
    tEvent ret;

    mutex.Lock();
    ret = mPresent;
    mutex.Unlock();

    return ret;
}

tEvent cGraphLCDState::GetFollowingEvent()
{
    tEvent ret;

    mutex.Lock();
    ret = mFollowing;
    mutex.Unlock();

    return ret;
}

tReplayState cGraphLCDState::GetReplayState()
{
    tReplayState ret;

    mutex.Lock();
    ret = mReplay;
    mutex.Unlock();

    return ret;
}

bool cGraphLCDState::IsRecording(int CardNumber)
{
    bool ret = false;
    std::vector <tRecording>::iterator it;

    mutex.Lock();
    if (CardNumber == -1 && mRecordings.size() > 0)
    {
        ret = true;
    }
    else
    {
        it = mRecordings.begin();
        while (it != mRecordings.end())
        {
            if (it->deviceNumber == CardNumber)
            {
                ret = true;
                break;
            }
            it++;
        }
    }
    mutex.Unlock();

    return ret;
}

std::string cGraphLCDState::Recordings(int CardNumber)
{
    std::string ret = "";
    std::vector <tRecording>::iterator it;

    mutex.Lock();
    it = mRecordings.begin();
    while (it != mRecordings.end())
    {
        if (CardNumber == -1 || it->deviceNumber == CardNumber)
        {
            if (ret.length() > 0)
                ret += "\n";
            ret += it->name;
        }
        it++;
    }
    mutex.Unlock();

    return ret;
}

tOsdState cGraphLCDState::GetOsdState()
{
    tOsdState ret;

    mutex.Lock();
    ret = mOsd;
    mutex.Unlock();

    return ret;
}

tVolumeState cGraphLCDState::GetVolumeState()
{
    tVolumeState ret;

    mutex.Lock();
    ret = mVolume;
    mutex.Unlock();

    return ret;
}

bool cGraphLCDState::ShowMessage()
{
    bool ret;

    mutex.Lock();
    ret = mOsd.message.length() > 0;
    mutex.Unlock();
    return ret;
}
