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
	tokPrivateChannelStart,
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
    tokChannelAlias,
    tokPrivateChannelEnd,

    // present event
    tokPrivatePresentStart,
    tokPresentStartDateTime,
    tokPresentVpsDateTime,
    tokPresentEndDateTime,
    tokPresentDuration,
    tokPresentProgress,
    tokPresentRemaining,
    tokPresentTitle,
    tokPresentShortText,
    tokPresentDescription,
    tokPrivatePresentEnd,

    // following event
    tokPrivateFollowingStart,
    tokFollowingStartDateTime,
    tokFollowingVpsDateTime,
    tokFollowingEndDateTime,
    tokFollowingDuration,
    tokFollowingTitle,
    tokFollowingShortText,
    tokFollowingDescription,
    tokPrivateFollowingEnd,

    // volume display
    tokPrivateVolumeStart,
    tokVolumeCurrent,
    tokVolumeTotal,
    tokIsMute,
    tokVolumeIsMute,
    tokPrivateVolumeEnd,

    tokPrivateReplayStart,
    tokReplayTitle,
    tokReplayPositionIndex,
    tokReplayDurationIndex,
    tokIsPlaying,
    tokReplayIsPlaying,
    tokIsFastForward,
    tokReplayIsFastForward,
    tokIsFastRewind,
    tokReplayIsFastRewind,
    tokIsSlowForward,
    tokReplayIsSlowForward,
    tokIsSlowRewind,
    tokReplayIsSlowRewind,
    tokIsPausing,
    tokReplayIsPausing,
    tokReplayPosition,
    tokReplayDuration,
    tokReplayRemaining,
    tokReplayMode,
    tokReplayIsShuffle,
    tokReplayIsLoop,
    tokPrivateReplayEnd,

    tokPrivateOsdStart,
    tokMessage,
    tokPrivateOsdEnd,

    tokDateTime,
    tokConfigPath,
    tokSkinPath,

    tokCountToken
} eTokenId;

static const std::string Tokens[tokCountToken] =
{
    "privateChannelStart",
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
    "ChannelAlias",
    "privateChannelEnd",

    "privatePresentStart",
    "PresentStartDateTime",
    "PresentVpsDateTime",
    "PresentEndDateTime",
    "PresentDuration",
    "PresentProgress",
    "PresentRemaining",
    "PresentTitle",
    "PresentShortText",
    "PresentDescription",
    "privatePresentEnd",

    "privateFollowingStart",
    "FollowingStartDateTime",
    "FollowingVpsDateTime",
    "FollowingEndDateTime",
    "FollowingDuration",
    "FollowingTitle",
    "FollowingShortText",
    "FollowingDescription",
    "privateFollowingEnd",

    "privateVolumeStart",
    "VolumeCurrent",
    "VolumeTotal",
    "IsMute",
    "VolumeIsMute",
    "privateVolumeEnd",

    "privateReplayStart",
    "ReplayTitle",
    "ReplayPositionIndex",
    "ReplayDurationIndex",
    "IsPlaying",
    "ReplayIsPlaying",
    "IsFastForward",
    "ReplayIsFastForward",
    "IsFastRewind",
    "ReplayIsFastRewind",
    "IsSlowForward",
    "ReplayIsSlowForward",
    "IsSlowRewind",
    "ReplayIsSlowRewind",
    "IsPausing",
    "ReplayIsPausing",
    "ReplayPosition",
    "ReplayDuration",
    "ReplayRemaining",
    "ReplayMode",
    "ReplayIsShuffle",
    "ReplayIsLoop",
    "privateReplayEnd",

    "privateOsdStart",
    "Message",
    "privateOsdEnd",

    "DateTime",
    "ConfigPath",
    "SkinPath"
};

cGraphLCDSkinConfig::cGraphLCDSkinConfig(const std::string & CfgPath, const std::string & SkinName, cGraphLCDState * State)
{
    mConfigPath = CfgPath;
    mSkinPath = CfgPath + "/skins/" + SkinName;
    mState = State;
    mAliasList.Load(CfgPath);
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
    if (Token.Id > tokPrivateChannelStart && Token.Id < tokPrivateChannelEnd)
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
            case tokChannelAlias:
            {
                char tmp[64];
                std::string alias;

                sprintf(tmp, "%d-%d-%d", channel.id.Nid(), channel.id.Tid(), channel.id.Sid());
                alias = mAliasList.GetAlias(tmp);
                return alias;
            }
            default:
                break;
        }
    }
    else if (Token.Id > tokPrivatePresentStart && Token.Id < tokPrivatePresentEnd)
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
            default:
                break;
        }
    }
    else if (Token.Id > tokPrivateFollowingStart && Token.Id < tokPrivateFollowingEnd)
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
            default:
                break;
        }
    }
    else if (Token.Id > tokPrivateVolumeStart && Token.Id < tokPrivateVolumeEnd)
    {
        tVolumeState volume = mState->GetVolumeState();
        switch (Token.Id)
        {
            case tokVolumeCurrent:
                return volume.value;
            case tokVolumeTotal:
                return 255;
            case tokIsMute:
            case tokVolumeIsMute:
                return volume.value == 0;
            default:
                break;
        }
    }
    else if (Token.Id > tokPrivateReplayStart && Token.Id < tokPrivateReplayEnd)
    {
        tReplayState replay = mState->GetReplayState();
        switch (Token.Id)
        {
            case tokReplayTitle:
                return replay.name;
            case tokReplayPositionIndex:
                return DurationType(replay.current, Token.Attrib.Text);
            case tokReplayDurationIndex:
                return DurationType(replay.total, Token.Attrib.Text);
            case tokReplayPosition:
                return replay.current;
            case tokReplayDuration:
                return replay.total;
            case tokReplayRemaining:
                return DurationType(replay.total - replay.current, Token.Attrib.Text);
            case tokIsPlaying:
            case tokReplayIsPlaying:
                return replay.play && replay.speed == -1;
            case tokIsPausing:
            case tokReplayIsPausing:
                return !replay.play && replay.speed == -1;
            case tokIsFastForward:
            case tokReplayIsFastForward:
                if (replay.play && replay.forward && replay.speed != -1)
                {
                    return Token.Attrib.Type == GLCD::aNumber
                        ? (GLCD::cType) (replay.speed == Token.Attrib.Number)
                        : (GLCD::cType) true;
                }
                return false;
            case tokIsFastRewind:
            case tokReplayIsFastRewind:
                if (replay.play && !replay.forward && replay.speed != -1)
                {
                    return Token.Attrib.Type == GLCD::aNumber
                        ? (GLCD::cType) (replay.speed == Token.Attrib.Number)
                        : (GLCD::cType) true;
                }
                return false;
            case tokIsSlowForward:
            case tokReplayIsSlowForward:
                if (!replay.play && replay.forward && replay.speed != -1)
                {
                    return Token.Attrib.Type == GLCD::aNumber
                        ? (GLCD::cType) (replay.speed == Token.Attrib.Number)
                        : (GLCD::cType) true;
                }
                return false;
            case tokIsSlowRewind:
            case tokReplayIsSlowRewind:
                if (!replay.play && !replay.forward && replay.speed != -1)
                {
                    return Token.Attrib.Type == GLCD::aNumber
                        ? (GLCD::cType) (replay.speed == Token.Attrib.Number)
                        : (GLCD::cType) true;
                }
                return false;
            case tokReplayMode:
            case tokReplayIsShuffle:
            case tokReplayIsLoop:
            default:
                break;
        }
    }
    else if (Token.Id > tokPrivateOsdStart && Token.Id < tokPrivateOsdEnd)
    {
        tOsdState osd = mState->GetOsdState();
        switch (Token.Id)
        {
            case tokMessage:
                return osd.message;
            default:
                break;
        }
    }
    else
    {
        switch (Token.Id)
        {
            case tokDateTime:
                return TimeType(time(NULL), Token.Attrib.Text);
            case tokConfigPath:
                return mConfigPath;
            case tokSkinPath:
                return mSkinPath;
            default:
                break;
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
