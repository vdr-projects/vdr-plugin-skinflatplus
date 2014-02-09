#pragma once

#include <string>
#include <vector>

#include <vdr/plugin.h>
#include "flat.h"

#define PLUGINCONFIGPATH (cPlugin::ConfigDirectory(PLUGIN_NAME_I18N))
#define PLUGINRESOURCEPATH (cPlugin::ResourceDirectory(PLUGIN_NAME_I18N))

class cFlatConfig
{
    private:
        cString checkSlashAtEnd(std::string path);

        int DecorCurrent;
    public:
        cFlatConfig(void);
        ~cFlatConfig(void);

        void Init(void);
        void SetLogoPath(cString path);
        bool SetupParse(const char *Name, const char *Value);
    
        void ThemeCheckAndInit(void);
        void ThemeInit(void);
        void DecorDescriptions(cStringList &Decors);
        cString DecorDescription(cString File);
        void DecorLoadFile(cString File);
        void DecorLoadCurrent(void);
        void DecorCheckAndInit(void);
    public:

        cString ThemeCurrent;
        cString logoPath;
        cString iconPath;
        
        // BORDER TYPES
        // 0 = none
        // 1 = rect
        // 2 = round
        // 3 = invert round
        // 4 = rect + alpha blend
        // 5 = round + alpha blend
        // 6 = invert round + alpha blend

        int decorBorderChannelByTheme;
        int decorBorderChannelTypeTheme, decorBorderChannelSizeTheme;
        int decorBorderChannelTypeUser, decorBorderChannelSizeUser;
        int decorBorderChannelType, decorBorderChannelSize;
        tColor decorBorderChannelFg, decorBorderChannelBg;

        int decorBorderTopBarByTheme;
        int decorBorderTopBarTypeTheme, decorBorderTopBarSizeTheme;
        int decorBorderTopBarTypeUser, decorBorderTopBarSizeUser;
        int decorBorderTopBarType, decorBorderTopBarSize;
        tColor decorBorderTopBarFg, decorBorderTopBarBg;

        int decorBorderMessageByTheme;
        int decorBorderMessageTypeTheme, decorBorderMessageSizeTheme;
        int decorBorderMessageTypeUser, decorBorderMessageSizeUser;
        int decorBorderMessageType, decorBorderMessageSize;
        tColor decorBorderMessageFg, decorBorderMessageBg;

        int decorBorderVolumeByTheme;
        int decorBorderVolumeTypeTheme, decorBorderVolumeSizeTheme;
        int decorBorderVolumeTypeUser, decorBorderVolumeSizeUser;
        int decorBorderVolumeType, decorBorderVolumeSize;
        tColor decorBorderVolumeFg, decorBorderVolumeBg;

        int decorBorderTrackByTheme;
        int decorBorderTrackTypeTheme, decorBorderTrackSizeTheme;
        int decorBorderTrackTypeUser, decorBorderTrackSizeUser;
        int decorBorderTrackType, decorBorderTrackSize;
        tColor decorBorderTrackFg, decorBorderTrackBg;
        tColor decorBorderTrackCurFg, decorBorderTrackCurBg;

        int decorBorderReplayByTheme;
        int decorBorderReplayTypeTheme, decorBorderReplaySizeTheme;
        int decorBorderReplayTypeUser, decorBorderReplaySizeUser;
        int decorBorderReplayType, decorBorderReplaySize;
        tColor decorBorderReplayFg, decorBorderReplayBg;

        int decorBorderMenuItemByTheme;
        int decorBorderMenuItemTypeTheme, decorBorderMenuItemSizeTheme;
        int decorBorderMenuItemTypeUser, decorBorderMenuItemSizeUser;
        int decorBorderMenuItemType, decorBorderMenuItemSize;
        tColor decorBorderMenuItemFg, decorBorderMenuItemBg;
        tColor decorBorderMenuItemSelFg, decorBorderMenuItemSelBg;
        tColor decorBorderMenuItemCurFg, decorBorderMenuItemCurBg;

        int decorBorderMenuContentHeadByTheme;
        int decorBorderMenuContentHeadTypeTheme, decorBorderMenuContentHeadSizeTheme;
        int decorBorderMenuContentHeadTypeUser, decorBorderMenuContentHeadSizeUser;
        int decorBorderMenuContentHeadType, decorBorderMenuContentHeadSize;
        tColor decorBorderMenuContentHeadFg, decorBorderMenuContentHeadBg;

        int decorBorderMenuContentByTheme;
        int decorBorderMenuContentTypeTheme, decorBorderMenuContentSizeTheme;
        int decorBorderMenuContentTypeUser, decorBorderMenuContentSizeUser;
        int decorBorderMenuContentType, decorBorderMenuContentSize;
        tColor decorBorderMenuContentFg, decorBorderMenuContentBg;

        int decorBorderButtonByTheme;
        int decorBorderButtonTypeTheme, decorBorderButtonSizeTheme;
        int decorBorderButtonTypeUser, decorBorderButtonSizeUser;
        int decorBorderButtonType, decorBorderButtonSize;
        tColor decorBorderButtonFg, decorBorderButtonBg;

        // PROGRESS TYPES
        // 0 = small line + big line
        // 1 = big line
        // 2 = big line + outline
        // 3 = small line + big line + dot
        // 4 = big line + dot
        // 5 = big line + outline + dot
        // 6 = small line + dot
        // 7 = outline + dot
        // 8 = small line + big line + alpha blend
        // 9 = big line + alpha blend
        int decorProgressChannelByTheme;
        int decorProgressChannelTypeTheme, decorProgressChannelSizeTheme;
        int decorProgressChannelTypeUser, decorProgressChannelSizeUser;
        int decorProgressChannelType, decorProgressChannelSize;
        tColor decorProgressChannelFg, decorProgressChannelBarFg, decorProgressChannelBg;

        int decorProgressVolumeByTheme;
        int decorProgressVolumeTypeTheme, decorProgressVolumeSizeTheme;
        int decorProgressVolumeTypeUser, decorProgressVolumeSizeUser;
        int decorProgressVolumeType, decorProgressVolumeSize;
        tColor decorProgressVolumeFg, decorProgressVolumeBarFg, decorProgressVolumeBg;

        int decorProgressMenuItemByTheme;
        int decorProgressMenuItemTypeTheme, decorProgressMenuItemSizeTheme;
        int decorProgressMenuItemTypeUser, decorProgressMenuItemSizeUser;
        int decorProgressMenuItemType, decorProgressMenuItemSize;
        tColor decorProgressMenuItemFg, decorProgressMenuItemBarFg, decorProgressMenuItemBg;
        tColor decorProgressMenuItemCurFg, decorProgressMenuItemCurBarFg, decorProgressMenuItemCurBg;

        int decorProgressReplayByTheme;
        int decorProgressReplayTypeTheme, decorProgressReplaySizeTheme;
        int decorProgressReplayTypeUser, decorProgressReplaySizeUser;
        int decorProgressReplayType, decorProgressReplaySize;
        tColor decorProgressReplayFg, decorProgressReplayBarFg, decorProgressReplayBg;

        int decorProgressSignalByTheme;
        int decorProgressSignalTypeTheme, decorProgressSignalSizeTheme;
        int decorProgressSignalTypeUser, decorProgressSignalSizeUser;
        int decorProgressSignalType, decorProgressSignalSize;
        tColor decorProgressSignalFg, decorProgressSignalBarFg, decorProgressSignalBg;

        // General Config
        int ButtonsShowEmpty;
        int ChannelIconsShow;
        int SignalQualityShow;
        int DiskUsageShow;
        
        int MenuItemPadding;
        int marginOsdVer, marginOsdHor;
        int MessageOffset;
        
        int MenuContentFullSize;
        double TopBarFontSize;
        
        int ChannelResolutionAspectShow;
        int ChannelFormatShow;
        int ChannelSimpleAspectFormat;
        int RecordingResolutionAspectShow;
        int RecordingFormatShow;
        int RecordingSimpleAspectFormat;
        int RecordingAdditionalInfoShow;
        int EpgAdditionalInfoShow;
        int TopBarRecordingShow;
        int MenuItemIconsShow;
        int TopBarMenuIconShow;
        
        double MainMenuItemScale;
        
        int MenuChannelView;
        
        int DecorIndex;
};
