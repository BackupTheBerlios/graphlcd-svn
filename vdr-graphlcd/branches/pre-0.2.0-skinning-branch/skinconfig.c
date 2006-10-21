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
#include <glcdskin/string.h>

#include "common.h"
#include "state.h"
#include "skinconfig.h"

typedef enum _eTokenId
{
	// current channel
	tokChannelNumber,
	tokChannelName,
	tokChannelShortName,
	tokChannelProvider,
	tokChannelPortal,
	tokChannelSource,
	tokChannelID,
	// present event
    tokPresentStartDateTime,
    tokPresentEndDateTime,
    tokPresentDuration,
    tokPresentProgress,
    tokPresentRemaining,
    tokPresentTitle,
    tokPresentShortText,
    tokPresentDescription,
    // following event
    tokFollowingStartDateTime,
    tokFollowingEndDateTime,
    tokFollowingDuration,
    tokFollowingTitle,
    tokFollowingShortText,
    tokFollowingDescription,

    tokCountToken
} eTokenId;

static const std::string Tokens[tokCountToken] =
{
	"ChannelNumber",
	"ChannelName",
	"ChannelShortName",
	"ChannelProvider",
	"ChannelPortal",
	"ChannelSource",
	"ChannelID",

    "PresentStartDateTime",
    "PresentEndDateTime",
    "PresentDuration",
    "PresentProgress",
    "PresentRemaining",
    "PresentTitle",
    "PresentShortText",
    "PresentDescription",

    "FollowingStartDateTime",
    "FollowingEndDateTime",
    "FollowingDuration",
    "FollowingTitle",
    "FollowingShortText",
    "FollowingDescription"
};

cGraphLCDSkinConfig::cGraphLCDSkinConfig(const std::string & CfgPath, const std::string & SkinName, cGraphLCDState * State)
{
    mSkinPath = CfgPath + "/skins/" + SkinName;
    mState = State;
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
    if (Token.Id >= tokChannelNumber && Token.Id <= tokChannelID)
    {
        tChannelState channel = mState->GetChannelState();
        switch (Token.Id)
        {
            case tokChannelNumber:
                return channel.number;
            case tokChannelName:
                return channel.str;
        }
    }
    else if (Token.Id >= tokPresentStartDateTime && Token.Id <= tokPresentDescription)
    {
        tEvent event = mState->GetPresentEvent();
        switch (Token.Id)
        {
            case tokPresentStartDateTime:
                return TimeType(event.startTime, Token.Attrib.Text);
            case tokPresentEndDateTime:
                return TimeType(event.startTime + event.duration, Token.Attrib.Text);
            case tokPresentDuration:
                return DurationType(event.duration * FRAMESPERSEC, Token.Attrib.Text);
            case tokPresentProgress:
                return DurationType((time(NULL) - event.startTime) * FRAMESPERSEC, Token.Attrib.Text);
            case tokPresentRemaining:
                if ((time(NULL) - event.startTime) < event.duration)
                {
                    return DurationType((event.duration - (time(NULL) - event.startTime)) * FRAMESPERSEC, Token.Attrib.Text);
                }
                return false;
            case tokPresentTitle:
                return event.title;
            case tokPresentShortText:
                return event.shortText;
            case tokPresentDescription:
                return event.description;
        }
    }
    else if (Token.Id >= tokFollowingStartDateTime && Token.Id <= tokFollowingDescription)
    {
        tEvent event = mState->GetFollowingEvent();
        switch (Token.Id)
        {
            case tokFollowingStartDateTime:
                return TimeType(event.startTime, Token.Attrib.Text);
            case tokFollowingEndDateTime:
                return TimeType(event.startTime + event.duration, Token.Attrib.Text);
            case tokFollowingDuration:
                return DurationType(event.duration * FRAMESPERSEC, Token.Attrib.Text);
            case tokFollowingTitle:
                return event.title;
            case tokFollowingShortText:
                return event.shortText;
            case tokFollowingDescription:
                return event.description;
        }
    }
    return "";
}

int cGraphLCDSkinConfig::GetTokenId(const std::string & Name) const
{
    int i;

    for (i = 0; i < tokCountToken; i++)
    {
        if (Name == Tokens[i])
            return i;
    }
    esyslog("graphlcd: unknown token %s", Name.c_str());
    return tokCountToken;
}
