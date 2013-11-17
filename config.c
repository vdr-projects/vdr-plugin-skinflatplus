#include "config.h"

cFlatConfig::cFlatConfig(void) {
    logoPath = "";
    iconPath = "";
    
    ButtonsShowEmpty = true;

    ChannelIconsShow = true;
    SignalQualityShow = true;
    
    DiskUsageShow = true;
    
    MenuContentFullSize = true;
    
    ResolutionAspectShow = false;
    
    MenuItemPadding = 5;
    marginOsdVer = 5;
    marginOsdHor = 5;
    TopBarFontSize = 0.05;
    MessageOffset = 50;
    
    decorBorderChannelByTheme = 1;
    decorBorderChannelTypeUser = 0;
    decorBorderChannelSizeUser = 0;

    decorBorderTopBarByTheme = 1;
    decorBorderTopBarTypeUser = 0;
    decorBorderTopBarSizeUser = 0;

    decorBorderMessageByTheme = 1;
    decorBorderMessageTypeUser = 0;
    decorBorderMessageSizeUser = 0;

    decorBorderVolumeByTheme = 1;
    decorBorderVolumeTypeUser = 0;
    decorBorderVolumeSizeUser = 0;

    decorBorderTrackByTheme = 1;
    decorBorderTrackTypeUser = 0;
    decorBorderTrackSizeUser = 0;

    decorBorderReplayByTheme = 1;
    decorBorderReplayTypeUser = 0;
    decorBorderReplaySizeUser = 0;

    decorBorderMenuItemByTheme = 1;
    decorBorderMenuItemTypeUser = 0;
    decorBorderMenuItemSizeUser = 0;

    decorBorderMenuContentHeadByTheme = 1;
    decorBorderMenuContentHeadTypeUser = 0;
    decorBorderMenuContentHeadSizeUser = 0;

    decorBorderMenuContentByTheme = 1;
    decorBorderMenuContentTypeUser = 0;
    decorBorderMenuContentSizeUser = 0;

    decorBorderButtonByTheme = 1;
    decorBorderButtonTypeUser = 0;
    decorBorderButtonSizeUser = 0;
    
    decorProgressChannelByTheme = 1;
    decorProgressChannelTypeUser = 0;
    decorProgressChannelSizeUser = 20;

    decorProgressVolumeByTheme = 1;
    decorProgressVolumeTypeUser = 0;
    decorProgressVolumeSizeUser = 20;

    decorProgressMenuItemByTheme = 1;
    decorProgressMenuItemTypeUser = 0;
    decorProgressMenuItemSizeUser = 20;

    decorProgressReplayByTheme = 1;
    decorProgressReplayTypeUser = 0;
    decorProgressReplaySizeUser = 40;

    decorProgressSignalByTheme = 1;
    decorProgressSignalTypeUser = 0;
    decorProgressSignalSizeUser = 20;

    ThemeCurrent = "";
    ThemeInit();
}

cFlatConfig::~cFlatConfig(void) {
}

bool cFlatConfig::SetupParse(const char *Name, const char *Value) {
    if      (strcmp(Name, "decorBorderChannelByTheme") == 0)            decorBorderChannelByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderChannelTypeUser") == 0)           decorBorderChannelTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderChannelSizeUser") == 0)           decorBorderChannelSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderTopBarByTheme") == 0)             decorBorderTopBarByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderTopBarTypeUser") == 0)            decorBorderTopBarTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderTopBarSizeUser") == 0)            decorBorderTopBarSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMessageByTheme") == 0)            decorBorderMessageByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderMessageTypeUser") == 0)           decorBorderMessageTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMessageSizeUser") == 0)           decorBorderMessageSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderVolumeByTheme") == 0)             decorBorderVolumeByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderVolumeTypeUser") == 0)            decorBorderVolumeTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderVolumeSizeUser") == 0)            decorBorderVolumeSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderTrackByTheme") == 0)              decorBorderTrackByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderTrackTypeUser") == 0)             decorBorderTrackTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderTrackSizeUser") == 0)             decorBorderTrackSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderReplayByTheme") == 0)             decorBorderReplayByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderReplayTypeUser") == 0)            decorBorderReplayTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderReplaySizeUser") == 0)            decorBorderReplaySizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuItemByTheme") == 0)           decorBorderMenuItemByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuItemTypeUser") == 0)          decorBorderMenuItemTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuItemSizeUser") == 0)          decorBorderMenuItemSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentHeadByTheme") == 0)    decorBorderMenuContentHeadByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentHeadTypeUser") == 0)   decorBorderMenuContentHeadTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentHeadSizeUser") == 0)   decorBorderMenuContentHeadSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentByTheme") == 0)        decorBorderMenuContentByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentTypeUser") == 0)       decorBorderMenuContentTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderMenuContentSizeUser") == 0)       decorBorderMenuContentSizeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderButtonByTheme") == 0)             decorBorderButtonByTheme = atoi(Value);
    else if (strcmp(Name, "decorBorderButtonTypeUser") == 0)            decorBorderButtonTypeUser = atoi(Value);
    else if (strcmp(Name, "decorBorderButtonSizeUser") == 0)            decorBorderButtonSizeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressChannelByTheme") == 0)          decorProgressChannelByTheme = atoi(Value);
    else if (strcmp(Name, "decorProgressChannelTypeUser") == 0)         decorProgressChannelTypeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressChannelSizeUser") == 0)         decorProgressChannelSizeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressVolumeByTheme") == 0)           decorProgressVolumeByTheme = atoi(Value);
    else if (strcmp(Name, "decorProgressVolumeTypeUser") == 0)          decorProgressVolumeTypeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressVolumeSizeUser") == 0)          decorProgressVolumeSizeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressMenuItemByTheme") == 0)         decorProgressMenuItemByTheme = atoi(Value);
    else if (strcmp(Name, "decorProgressMenuItemTypeUser") == 0)        decorProgressMenuItemTypeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressMenuItemSizeUser") == 0)        decorProgressMenuItemSizeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressReplayByTheme") == 0)           decorProgressReplayByTheme = atoi(Value);
    else if (strcmp(Name, "decorProgressReplayTypeUser") == 0)          decorProgressReplayTypeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressReplaySizeUser") == 0)          decorProgressReplaySizeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressSignalByTheme") == 0)           decorProgressSignalByTheme = atoi(Value);
    else if (strcmp(Name, "decorProgressSignalTypeUser") == 0)          decorProgressSignalTypeUser = atoi(Value);
    else if (strcmp(Name, "decorProgressSignalSizeUser") == 0)          decorProgressSignalSizeUser = atoi(Value);
    else if (strcmp(Name, "ButtonsShowEmpty") == 0)                     ButtonsShowEmpty = atoi(Value);
    else if (strcmp(Name, "ChannelIconsShow") == 0)                     ChannelIconsShow = atoi(Value);
    else if (strcmp(Name, "SignalQualityShow") == 0)                    SignalQualityShow = atoi(Value);
    else if (strcmp(Name, "DiskUsageShow") == 0)                        DiskUsageShow = atoi(Value);
    else if (strcmp(Name, "MenuItemPadding") == 0)                      MenuItemPadding = atoi(Value);
    else if (strcmp(Name, "marginOsdVer") == 0)                         marginOsdVer = atoi(Value);
    else if (strcmp(Name, "marginOsdHor") == 0)                         marginOsdHor = atoi(Value);
    else if (strcmp(Name, "MessageOffset") == 0)                        MessageOffset = atoi(Value);
    else if (strcmp(Name, "TopBarFontSize") == 0)                       TopBarFontSize = atod(Value);
    else if (strcmp(Name, "MenuContentFullSize") == 0)                  MenuContentFullSize = atoi(Value);
    else if (strcmp(Name, "ResolutionAspectShow") == 0)                 ResolutionAspectShow = atoi(Value);
    else return false;
    return true;
}

void cFlatConfig::ThemeCheckAndInit(void) {
    if( strcmp(Theme.Name(), *ThemeCurrent) != 0 )
    {
        ThemeCurrent = Theme.Name();
        ThemeInit();
    }
}

void cFlatConfig::ThemeInit(void) {
    decorBorderChannelTypeTheme = Theme.Color(clrChannelBorderType) & 0x000000FF;
    decorBorderChannelSizeTheme = Theme.Color(clrChannelBorderSize) & 0x000000FF;
    decorBorderChannelFg = Theme.Color(clrChannelBorderFg);
    decorBorderChannelBg = Theme.Color(clrChannelBorderBg);

    decorBorderTopBarTypeTheme = Theme.Color(clrTopBarBorderType) & 0x000000FF;
    decorBorderTopBarSizeTheme = Theme.Color(clrTopBarBorderSize) & 0x000000FF;
    decorBorderTopBarFg = Theme.Color(clrTopBarBorderFg);
    decorBorderTopBarBg = Theme.Color(clrTopBarBorderBg);

    decorBorderMessageTypeTheme = Theme.Color(clrMessageBorderType) & 0x000000FF;
    decorBorderMessageSizeTheme = Theme.Color(clrMessageBorderSize) & 0x000000FF;
    decorBorderMessageFg = Theme.Color(clrMessageBorderFg);
    decorBorderMessageBg = Theme.Color(clrMessageBorderBg);

    decorBorderVolumeTypeTheme = Theme.Color(clrVolumeBorderType) & 0x000000FF;
    decorBorderVolumeSizeTheme = Theme.Color(clrVolumeBorderSize) & 0x000000FF;
    decorBorderVolumeFg = Theme.Color(clrVolumeBorderFg);
    decorBorderVolumeBg = Theme.Color(clrVolumeBorderBg);

    decorBorderTrackTypeTheme = Theme.Color(clrTrackItemBorderType) & 0x000000FF;
    decorBorderTrackSizeTheme = Theme.Color(clrTrackItemBorderSize) & 0x000000FF;
    decorBorderTrackFg = Theme.Color(clrTrackItemBorderFg);
    decorBorderTrackBg = Theme.Color(clrTrackItemBorderBg);
    decorBorderTrackCurFg = Theme.Color(clrTrackItemCurrentBorderFg);
    decorBorderTrackCurBg = Theme.Color(clrTrackItemCurrentBorderBg);

    decorBorderReplayTypeTheme = Theme.Color(clrReplayBorderType) & 0x000000FF;
    decorBorderReplaySizeTheme = Theme.Color(clrReplayBorderSize) & 0x000000FF;
    decorBorderReplayFg = Theme.Color(clrReplayBorderFg);
    decorBorderReplayBg = Theme.Color(clrReplayBorderBg);

    decorBorderMenuItemTypeTheme = Theme.Color(clrMenuItemBorderType) & 0x000000FF;
    decorBorderMenuItemSizeTheme = Theme.Color(clrMenuItemBorderSize) & 0x000000FF;
    decorBorderMenuItemFg = Theme.Color(clrMenuItemBorderFg);
    decorBorderMenuItemBg = Theme.Color(clrMenuItemBorderBg);
    decorBorderMenuItemSelFg = Theme.Color(clrMenuItemSelableBorderFg);
    decorBorderMenuItemSelBg = Theme.Color(clrMenuItemSelableBorderBg);
    decorBorderMenuItemCurFg = Theme.Color(clrMenuItemCurrentBorderFg);
    decorBorderMenuItemCurBg = Theme.Color(clrMenuItemCurrentBorderBg);

    decorBorderMenuContentHeadTypeTheme = Theme.Color(clrMenuContentHeadBorderType) & 0x000000FF;
    decorBorderMenuContentHeadSizeTheme = Theme.Color(clrMenuContentHeadBorderSize) & 0x000000FF;
    decorBorderMenuContentHeadFg = Theme.Color(clrMenuContentHeadBorderFg);
    decorBorderMenuContentHeadBg = Theme.Color(clrMenuContentHeadBorderBg);

    decorBorderMenuContentTypeTheme = Theme.Color(clrMenuContentBorderType) & 0x000000FF;
    decorBorderMenuContentSizeTheme = Theme.Color(clrMenuContentBorderSize) & 0x000000FF;
    decorBorderMenuContentFg = Theme.Color(clrMenuContentBorderFg);
    decorBorderMenuContentBg = Theme.Color(clrMenuContentBorderBg);

    decorBorderButtonTypeTheme = Theme.Color(clrButtonBorderType) & 0x000000FF;
    decorBorderButtonSizeTheme = Theme.Color(clrButtonBorderSize) & 0x000000FF;
    decorBorderButtonFg = Theme.Color(clrButtonBorderFg);
    decorBorderButtonBg = Theme.Color(clrButtonBorderBg);
    
    decorProgressChannelTypeTheme = Theme.Color(clrChannelProgressType) & 0x000000FF;
    decorProgressChannelSizeTheme = Theme.Color(clrChannelProgressSize) & 0x000000FF;
    decorProgressChannelFg = Theme.Color(clrChannelProgressFg);
    decorProgressChannelBarFg = Theme.Color(clrChannelProgressBarFg);
    decorProgressChannelBg = Theme.Color(clrChannelProgressBg);

    decorProgressVolumeTypeTheme = Theme.Color(clrVolumeProgressType) & 0x000000FF;
    decorProgressVolumeSizeTheme = Theme.Color(clrVolumeProgressSize) & 0x000000FF;
    decorProgressVolumeFg = Theme.Color(clrVolumeProgressFg);
    decorProgressVolumeBarFg = Theme.Color(clrVolumeProgressBarFg);
    decorProgressVolumeBg = Theme.Color(clrVolumeProgressBg);

    decorProgressMenuItemTypeTheme = Theme.Color(clrMenuItemProgressType) & 0x000000FF;
    decorProgressMenuItemSizeTheme = Theme.Color(clrMenuItemProgressSize) & 0x000000FF;
    decorProgressMenuItemFg = Theme.Color(clrMenuItemProgressFg);
    decorProgressMenuItemBarFg = Theme.Color(clrMenuItemProgressBarFg);
    decorProgressMenuItemBg = Theme.Color(clrMenuItemProgressBg);
    decorProgressMenuItemCurFg = Theme.Color(clrMenuItemCurProgressFg);
    decorProgressMenuItemCurBarFg = Theme.Color(clrMenuItemCurProgressBarFg);
    decorProgressMenuItemCurBg = Theme.Color(clrMenuItemCurProgressBg);

    decorProgressReplayTypeTheme = Theme.Color(clrReplayProgressType) & 0x000000FF;
    decorProgressReplaySizeTheme = Theme.Color(clrReplayProgressSize) & 0x000000FF;
    decorProgressReplayFg = Theme.Color(clrReplayProgressFg);
    decorProgressReplayBarFg = Theme.Color(clrReplayProgressBarFg);
    decorProgressReplayBg = Theme.Color(clrReplayProgressBg);

    decorProgressSignalTypeTheme = Theme.Color(clrChannelSignalProgressType) & 0x000000FF;
    decorProgressSignalSizeTheme = Theme.Color(clrChannelSignalProgressSize) & 0x000000FF;
    decorProgressSignalFg = Theme.Color(clrChannelSignalProgressFg);
    decorProgressSignalBarFg = Theme.Color(clrChannelSignalProgressBarFg);
    decorProgressSignalBg = Theme.Color(clrChannelSignalProgressBg);

    if( decorBorderChannelByTheme ) {
        decorBorderChannelType = decorBorderChannelTypeTheme;
        decorBorderChannelSize = decorBorderChannelSizeTheme;
    } else {
        decorBorderChannelType = decorBorderChannelTypeUser;
        decorBorderChannelSize = decorBorderChannelSizeUser;
    }
    
    if( decorBorderTopBarByTheme ) {
        decorBorderTopBarType = decorBorderTopBarTypeTheme;
        decorBorderTopBarSize = decorBorderTopBarSizeTheme;
    } else {
        decorBorderTopBarType = decorBorderTopBarTypeUser;
        decorBorderTopBarSize = decorBorderTopBarSizeUser;
    }

    if( decorBorderMessageByTheme ) {
        decorBorderMessageType = decorBorderMessageTypeTheme;
        decorBorderMessageSize = decorBorderMessageSizeTheme;
    } else {
        decorBorderMessageType = decorBorderMessageTypeUser;
        decorBorderMessageSize = decorBorderMessageSizeUser;
    }
    
    if( decorBorderVolumeByTheme ) {
        decorBorderVolumeType = decorBorderVolumeTypeTheme;
        decorBorderVolumeSize = decorBorderVolumeSizeTheme;
    } else {
        decorBorderVolumeType = decorBorderVolumeTypeUser;
        decorBorderVolumeSize = decorBorderVolumeSizeUser;
    }
    
    if( decorBorderTrackByTheme ) {
        decorBorderTrackType = decorBorderTrackTypeTheme;
        decorBorderTrackSize = decorBorderTrackSizeTheme;
    } else {
        decorBorderTrackType = decorBorderTrackTypeUser;
        decorBorderTrackSize = decorBorderTrackSizeUser;
    }
    
    if( decorBorderReplayByTheme ) {
        decorBorderReplayType = decorBorderReplayTypeTheme;
        decorBorderReplaySize = decorBorderReplaySizeTheme;
    } else {
        decorBorderReplayType = decorBorderReplayTypeUser;
        decorBorderReplaySize = decorBorderReplaySizeUser;
    }

    if( decorBorderMenuItemByTheme ) {
        decorBorderMenuItemType = decorBorderMenuItemTypeTheme;
        decorBorderMenuItemSize = decorBorderMenuItemSizeTheme;
    } else {
        decorBorderMenuItemType = decorBorderMenuItemTypeUser;
        decorBorderMenuItemSize = decorBorderMenuItemSizeUser;
    }
    
    if( decorBorderMenuContentHeadByTheme ) {
        decorBorderMenuContentHeadType = decorBorderMenuContentHeadTypeTheme;
        decorBorderMenuContentHeadSize = decorBorderMenuContentHeadSizeTheme;
    } else {
        decorBorderMenuContentHeadType = decorBorderMenuContentHeadTypeUser;
        decorBorderMenuContentHeadSize = decorBorderMenuContentHeadSizeUser;
    }
    
    if( decorBorderMenuContentByTheme ) {
        decorBorderMenuContentType = decorBorderMenuContentTypeTheme;
        decorBorderMenuContentSize = decorBorderMenuContentSizeTheme;
    } else {
        decorBorderMenuContentType = decorBorderMenuContentTypeUser;
        decorBorderMenuContentSize = decorBorderMenuContentSizeUser;
    }
    
    if( decorBorderButtonByTheme ) {
        decorBorderButtonType = decorBorderButtonTypeTheme;
        decorBorderButtonSize = decorBorderButtonSizeTheme;
    } else {
        decorBorderButtonType = decorBorderButtonTypeUser;
        decorBorderButtonSize = decorBorderButtonSizeUser;
    }
    
    if( decorProgressChannelByTheme ) {
        decorProgressChannelType = decorProgressChannelTypeTheme;
        decorProgressChannelSize = decorProgressChannelSizeTheme;
    } else {
        decorProgressChannelType = decorProgressChannelTypeUser;
        decorProgressChannelSize = decorProgressChannelSizeUser;
    }
    
    if( decorProgressVolumeByTheme ) {
        decorProgressVolumeType = decorProgressVolumeTypeTheme;
        decorProgressVolumeSize = decorProgressVolumeSizeTheme;
    } else {
        decorProgressVolumeType = decorProgressVolumeTypeUser;
        decorProgressVolumeSize = decorProgressVolumeSizeUser;
    }
    
    if( decorProgressMenuItemByTheme ) {
        decorProgressMenuItemType = decorProgressMenuItemTypeTheme;
        decorProgressMenuItemSize = decorProgressMenuItemSizeTheme;
    } else {
        decorProgressMenuItemType = decorProgressMenuItemTypeUser;
        decorProgressMenuItemSize = decorProgressMenuItemSizeUser;
    }
    
    if( decorProgressReplayByTheme ) {
        decorProgressReplayType = decorProgressReplayTypeTheme;
        decorProgressReplaySize = decorProgressReplaySizeTheme;
    } else {
        decorProgressReplayType = decorProgressReplayTypeUser;
        decorProgressReplaySize = decorProgressReplaySizeUser;
    }
    
    if( decorProgressSignalByTheme ) {
        decorProgressSignalType = decorProgressSignalTypeTheme;
        decorProgressSignalSize = decorProgressSignalSizeTheme;
    } else {
        decorProgressSignalType = decorProgressSignalTypeUser;
        decorProgressSignalSize = decorProgressSignalSizeUser;
    }
    
    if( decorBorderChannelType == 0 )
        decorBorderChannelSize = 0;
    if( decorBorderTopBarType == 0 )
        decorBorderTopBarSize = 0;
    if( decorBorderMessageType == 0 )
        decorBorderMessageSize = 0;
    if( decorBorderVolumeType == 0 )
        decorBorderVolumeSize = 0;
    if( decorBorderTrackType == 0 )
        decorBorderTrackSize = 0;
    if( decorBorderReplayType == 0 )
        decorBorderReplaySize = 0;
    if( decorBorderMenuItemType == 0 )
        decorBorderMenuItemSize = 0;
    if( decorBorderMenuContentHeadType == 0 )
        decorBorderMenuContentHeadSize = 0;
    if( decorBorderMenuContentType == 0 )
        decorBorderMenuContentSize = 0;
    if( decorBorderButtonType == 0 )
        decorBorderButtonSize = 0;
}

void cFlatConfig::Init(void) {
    if( !strcmp(logoPath, "") )
        logoPath = cString::sprintf("%s/logos/", PLUGINRESOURCEPATH);
    //dsyslog("skinflat: logopath: %s", *logoPath);
    if( !strcmp(iconPath, "") )
        iconPath = cString::sprintf("%s/icons/", PLUGINRESOURCEPATH);
    //dsyslog("skinflat: iconPath: %s", *iconPath);

    ThemeInit();
}

void cFlatConfig::SetLogoPath(cString path) {
    logoPath = checkSlashAtEnd(*path);
}

cString cFlatConfig::checkSlashAtEnd(std::string path) {
    try {
        if (!(path.at(path.size()-1) == '/'))
            return cString::sprintf("%s/", path.c_str());
    } catch (...) {return path.c_str();}
    return path.c_str();
}
