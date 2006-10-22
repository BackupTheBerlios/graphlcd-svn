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
    tokHasTeletext,
    tokChannelHasTeletext,
    tokHasMultilang,
    tokChannelHasMultilang,
    tokHasDolby,
    tokChannelHasDolby,
    tokIsEncrypted,
    tokChannelIsEncrypted,
    tokIsRadio,
    tokChannelIsRadio,
    // present event
    tokPresentStartDateTime,
    tokPresentVpsDateTime,
    tokPresentEndDateTime,
    tokPresentDuration,
    tokPresentProgress,
    tokPresentRemaining,
    tokPresentTitle,
    tokPresentShortText,
    tokPresentDescription,
    // following event
    tokFollowingStartDateTime,
    tokFollowingVpsDateTime,
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
    "HasTeletext",
    "ChannelHasTeletext",
    "HasMultilang",
    "ChannelHasMultilang",
    "HasDolby",
    "ChannelHasDolby",
    "IsEncrypted",
    "ChannelIsEncrypted",
    "IsRadio",
    "ChannelIsRadio",

    "PresentStartDateTime",
    "PresentVpsDateTime",
    "PresentEndDateTime",
    "PresentDuration",
    "PresentProgress",
    "PresentRemaining",
    "PresentTitle",
    "PresentShortText",
    "PresentDescription",

    "FollowingStartDateTime",
    "FollowingVpsDateTime",
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
    if (Token.Id >= tokChannelNumber && Token.Id <= tokChannelIsRadio)
    {
        tChannel channel = mState->GetChannelInfo();
        switch (Token.Id)
        {
            case tokChannelNumber:
                return channel.number;
            case tokChannelName:
                return channel.name;
            case tokChannelShortName:
                return channel.shortName;
            case tokChannelProvider:
                return channel.provider;
            case tokChannelPortal:
                return channel.portal;
            case tokChannelSource:
                return channel.source;
            case tokChannelID:
                return (GLCD::cType) channel.id.ToString();
            case tokHasTeletext:
            case tokChannelHasTeletext:
                return channel.hasTeletext;
            case tokHasMultilang:
            case tokChannelHasMultilang:
                return channel.hasMultiLanguage;
            case tokHasDolby:
            case tokChannelHasDolby:
                return channel.hasDolby;
            case tokIsEncrypted:
            case tokChannelIsEncrypted:
                return channel.isEncrypted;
            case tokIsRadio:
            case tokChannelIsRadio:
                return channel.isRadio;
        }
    }
    else if (Token.Id >= tokPresentStartDateTime && Token.Id <= tokPresentDescription)
    {
        tEvent event = mState->GetPresentEvent();
        switch (Token.Id)
        {
            case tokPresentStartDateTime:
                return TimeType(event.startTime, Token.Attrib.Text);
            case tokPresentVpsDateTime:
                return TimeType(event.vpsTime, Token.Attrib.Text);
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
            case tokFollowingVpsDateTime:
                return TimeType(event.vpsTime, Token.Attrib.Text);
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
