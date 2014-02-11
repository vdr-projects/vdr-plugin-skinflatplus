#pragma once

#include <vdr/skins.h>
#include <vdr/videodir.h>

#include "config.h"
#include "imagecache.h"

extern class cFlatConfig Config;
extern class cImageCache imgCache;
extern bool firstDisplay;

class cFlatDisplayMenu;
extern cTheme Theme;

// BUTTONS
#define CLR_BUTTONRED                       0xFFBB0000
#define CLR_BUTTONGREEN                     0xFF00BB00
#define CLR_BUTTONYELLOW                    0xFFBBBB00
#define CLR_BUTTONBLUE                      0xFF0000BB

// MESSAGES
#define CLR_MESSAGESTATUS                   0xFF0000FF
#define CLR_MESSAGEINFO                     0xFF009900
#define CLR_MESSAGEWARNING                  0xFFBBBB00
#define CLR_MESSAGEERROR                    0xFFBB0000

// TopBar
THEME_CLR(Theme, clrTopBarBg,               0xC0101010);
THEME_CLR(Theme, clrTopBarFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrTopBarTimeFont,         0xFFEEEEEE);
THEME_CLR(Theme, clrTopBarDateFont,         0xFFA0A0A0);
THEME_CLR(Theme, clrTopBarBorderFg,         0xC0101010);
THEME_CLR(Theme, clrTopBarBorderBg,         0xC0101010);

THEME_CLR(Theme, clrTopBarRecordingActiveFg,  0xFF880000);
THEME_CLR(Theme, clrTopBarRecordingActiveBg,  0xC0101010);

// Buttons
THEME_CLR(Theme, clrButtonBg,               0xC0101010);
THEME_CLR(Theme, clrButtonFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrButtonRed,              CLR_BUTTONRED);
THEME_CLR(Theme, clrButtonGreen,            CLR_BUTTONGREEN);
THEME_CLR(Theme, clrButtonYellow,           CLR_BUTTONYELLOW);
THEME_CLR(Theme, clrButtonBlue,             CLR_BUTTONBLUE);

THEME_CLR(Theme, clrButtonBorderFg,        0xC0101010);
THEME_CLR(Theme, clrButtonBorderBg,        0xC0101010);

// Messages
THEME_CLR(Theme, clrMessageBg,              0xC0101010);
THEME_CLR(Theme, clrMessageFont,            0xFFEEEEEE);

THEME_CLR(Theme, clrMessageStatus,          CLR_MESSAGESTATUS);
THEME_CLR(Theme, clrMessageInfo,            CLR_MESSAGEINFO);
THEME_CLR(Theme, clrMessageWarning,         CLR_MESSAGEWARNING);
THEME_CLR(Theme, clrMessageError,           CLR_MESSAGEERROR);

THEME_CLR(Theme, clrMessageBorderFg,        0xC0101010);
THEME_CLR(Theme, clrMessageBorderBg,        0xC0101010);

// Channel
THEME_CLR(Theme, clrChannelBg,              0xC0101010);
THEME_CLR(Theme, clrChannelFontTitle,       0xFF3090B0);
THEME_CLR(Theme, clrChannelFontEpg,         0xFFEEEEEE);
THEME_CLR(Theme, clrChannelFontEpgFollow,   0xFFA0A0A0);
THEME_CLR(Theme, clrChannelProgressFg,      0xFF3090B0);
THEME_CLR(Theme, clrChannelProgressBarFg,   0xFF3090B0);
THEME_CLR(Theme, clrChannelProgressBg,      0xC0101010);

THEME_CLR(Theme, clrChannelBorderFg,        0xC03090B0);
THEME_CLR(Theme, clrChannelBorderBg,        0xC03090B0);

THEME_CLR(Theme, clrChannelSignalFont,            0xFFA0A0A0);
THEME_CLR(Theme, clrChannelSignalProgressFg,      0xFFA0A0A0);
THEME_CLR(Theme, clrChannelSignalProgressBarFg,   0xFFA0A0A0);
THEME_CLR(Theme, clrChannelSignalProgressBg,      0xC0101010);

THEME_CLR(Theme, clrChannelRecordingPresentFg,    0xFFAA0000);
THEME_CLR(Theme, clrChannelRecordingPresentBg,    0xC0101010);
THEME_CLR(Theme, clrChannelRecordingFollowFg,     0xFFA0A0A0);
THEME_CLR(Theme, clrChannelRecordingFollowBg,     0xC0101010);

// Menu
THEME_CLR(Theme, clrItemBg,                 0xC0909090);
THEME_CLR(Theme, clrItemFont,               0xFFEEEEEE);
THEME_CLR(Theme, clrItemCurrentBg,          0xC03090B0);
THEME_CLR(Theme, clrItemCurrentFont,        0xFFEEEEEE);
THEME_CLR(Theme, clrItemSelableBg,          0xC0101010);
THEME_CLR(Theme, clrItemSelableFont,        0xFFEEEEEE);
THEME_CLR(Theme, clrScrollbarFg,            0xFF3090B0);
THEME_CLR(Theme, clrScrollbarBarFg,         0xFF3090B0);
THEME_CLR(Theme, clrScrollbarBg,            0xC0101010);
// Menu Event
THEME_CLR(Theme, clrMenuEventBg,            0xC0101010);
THEME_CLR(Theme, clrMenuEventFontTitle,     0xFF3090B0);
THEME_CLR(Theme, clrMenuEventFontInfo,      0xFFEEEEEE);
// Menu Recording
THEME_CLR(Theme, clrMenuRecBg,              0xC0101010);
THEME_CLR(Theme, clrMenuRecFontTitle,       0xFF3090B0);
THEME_CLR(Theme, clrMenuRecFontInfo,        0xFFEEEEEE);
// Menu Text (Multiline)
THEME_CLR(Theme, clrMenuTextBg,             0xC0101010);
THEME_CLR(Theme, clrMenuTextFont,           0xFFEEEEEE);

THEME_CLR(Theme, clrMenuContentHeadBorderFg,        0xC03090B0);
THEME_CLR(Theme, clrMenuContentHeadBorderBg,        0xC03090B0);

THEME_CLR(Theme, clrMenuContentBorderFg,        0xC03090B0);
THEME_CLR(Theme, clrMenuContentBorderBg,        0xC03090B0);

// Menu Items
THEME_CLR(Theme, clrMenuItemProgressFg,      0xFFEEEEEE);
THEME_CLR(Theme, clrMenuItemProgressBarFg,   0xFFEEEEEE);
THEME_CLR(Theme, clrMenuItemProgressBg,      0xC0101010);
THEME_CLR(Theme, clrMenuItemCurProgressFg,      0xFFEEEEEE);
THEME_CLR(Theme, clrMenuItemCurProgressBarFg,   0xFFEEEEEE);
THEME_CLR(Theme, clrMenuItemCurProgressBg,      0xC03090B0);

THEME_CLR(Theme, clrMenuItemBorderFg,      0xC0909090);
THEME_CLR(Theme, clrMenuItemBorderBg,      0xC0909090);
THEME_CLR(Theme, clrMenuItemSelableBorderFg,      0xC0101010);
THEME_CLR(Theme, clrMenuItemSelableBorderBg,      0xC0101010);
THEME_CLR(Theme, clrMenuItemCurrentBorderFg,      0xC03090B0);
THEME_CLR(Theme, clrMenuItemCurrentBorderBg,      0xC03090B0);

// Menu Timer Item
THEME_CLR(Theme, clrMenuTimerItemDisabledFont,       0xFFA0A0A0);
THEME_CLR(Theme, clrMenuTimerItemRecordingFont,      0xFFEEEEEE);

// Replay
THEME_CLR(Theme, clrReplayBg,               0xC0101010);
THEME_CLR(Theme, clrReplayFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrReplayFontSpeed,        0xFF3090B0);
THEME_CLR(Theme, clrReplayProgressFg,       0xFFEEEEEE);
THEME_CLR(Theme, clrReplayProgressBarFg,    0xFFEEEEEE);
THEME_CLR(Theme, clrReplayProgressBarCurFg, 0xFF3090B0);
THEME_CLR(Theme, clrReplayProgressBg,       0xC0101010);
THEME_CLR(Theme, clrReplayMarkFg,           0xFFEEEEEE);
THEME_CLR(Theme, clrReplayMarkCurrentFg,    0xFF3090B0);

THEME_CLR(Theme, clrReplayBorderFg,         0xC0101010);
THEME_CLR(Theme, clrReplayBorderBg,         0xC0101010);

// Tracks
THEME_CLR(Theme, clrTrackItemBg,            0xC0101010);
THEME_CLR(Theme, clrTrackItemFont,          0xFFEEEEEE);
THEME_CLR(Theme, clrTrackItemCurrentBg,     0xC03090B0);
THEME_CLR(Theme, clrTrackItemCurrentFont,   0xFFEEEEEE);

THEME_CLR(Theme, clrTrackItemBorderFg,      0xC0101010);
THEME_CLR(Theme, clrTrackItemBorderBg,      0xC0101010);
THEME_CLR(Theme, clrTrackItemCurrentBorderFg,      0xC03090B0);
THEME_CLR(Theme, clrTrackItemCurrentBorderBg,      0xC03090B0);

// Volume
THEME_CLR(Theme, clrVolumeBg,               0xC0101010);
THEME_CLR(Theme, clrVolumeFont,             0xFFEEEEEE);
THEME_CLR(Theme, clrVolumeProgressFg,       0xFF3090B0);
THEME_CLR(Theme, clrVolumeProgressBarFg,    0xFF3090B0);
THEME_CLR(Theme, clrVolumeProgressBg,       0xC0101010);

THEME_CLR(Theme, clrVolumeBorderFg,         0xC0101010);
THEME_CLR(Theme, clrVolumeBorderBg,         0xC0101010);

class cFlat : public cSkin {
    private:
        cFlatDisplayMenu *displayMenu;
    public:
        cFlat(void);
        virtual const char *Description(void);
        virtual cSkinDisplayChannel *DisplayChannel(bool WithInfo);
        virtual cSkinDisplayMenu *DisplayMenu(void);
        virtual cSkinDisplayReplay *DisplayReplay(bool ModeOnly);
        virtual cSkinDisplayVolume *DisplayVolume(void);
        virtual cSkinDisplayTracks *DisplayTracks(const char *Title, int NumTracks, const char * const *Tracks);
        virtual cSkinDisplayMessage *DisplayMessage(void);
};
