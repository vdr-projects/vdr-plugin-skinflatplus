#include "config.h"

#include <vector>
#include <algorithm>
cFlatConfig::cFlatConfig(void) {
    logoPath = "";
    iconPath = "";
    
    DecorCurrent = -1;
    DecorIndex = 0;
    
    ButtonsShowEmpty = true;

    ChannelIconsShow = true;
    SignalQualityShow = true;
    
    DiskUsageShow = true;
    
    MenuContentFullSize = true;
    
    ChannelFormatShow = true;
    ChannelResolutionAspectShow = true;
    ChannelSimpleAspectFormat = true;
    
    RecordingResolutionAspectShow = true;
    RecordingFormatShow = true;
    RecordingSimpleAspectFormat = true;
    
    RecordingAdditionalInfoShow = true;
    EpgAdditionalInfoShow = true;
    
    TopBarRecordingShow = true;
    
    MenuItemIconsShow = true;
    TopBarMenuIconShow = true;
    
    MenuItemPadding = 3;
    marginOsdVer = 5;
    marginOsdHor = 5;
    TopBarFontSize = 0.05;
    MessageOffset = 50;
    
    MainMenuItemScale = 1.0;
    
    MenuChannelView = 1;
    MenuTimerView = 3;
    MenuEventView = 1;
    MenuRecordingView = 1;
    
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
    else if (strcmp(Name, "ChannelResolutionAspectShow") == 0)          ChannelResolutionAspectShow = atoi(Value);
    else if (strcmp(Name, "ChannelFormatShow") == 0)                    ChannelFormatShow = atoi(Value);
    else if (strcmp(Name, "RecordingResolutionAspectShow") == 0)        RecordingResolutionAspectShow = atoi(Value);
    else if (strcmp(Name, "RecordingFormatShow") == 0)                  RecordingFormatShow = atoi(Value);
    else if (strcmp(Name, "RecordingAdditionalInfoShow") == 0)          RecordingAdditionalInfoShow = atoi(Value);
    else if (strcmp(Name, "EpgAdditionalInfoShow") == 0)                EpgAdditionalInfoShow = atoi(Value);
    else if (strcmp(Name, "TopBarRecordingShow") == 0)                  TopBarRecordingShow = atoi(Value);
    else if (strcmp(Name, "MenuItemIconsShow") == 0)                    MenuItemIconsShow = atoi(Value);
    else if (strcmp(Name, "TopBarMenuIconShow") == 0)                   TopBarMenuIconShow = atoi(Value);
    else if (strcmp(Name, "DecorIndex") == 0)                           DecorIndex = atoi(Value);
    else if (strcmp(Name, "MainMenuItemScale") == 0)                    MainMenuItemScale = atod(Value);
    else if (strcmp(Name, "MenuChannelView") == 0)                      MenuChannelView = atoi(Value);
    else if (strcmp(Name, "MenuTimerView") == 0)                        MenuTimerView = atoi(Value);
    else if (strcmp(Name, "MenuEventView") == 0)                        MenuEventView = atoi(Value);
    else if (strcmp(Name, "MenuRecordingView") == 0)                    MenuRecordingView = atoi(Value);
    else if (strcmp(Name, "ChannelSimpleAspectFormat") == 0)            ChannelSimpleAspectFormat = atoi(Value);
    else if (strcmp(Name, "RecordingSimpleAspectFormat") == 0)          RecordingSimpleAspectFormat = atoi(Value);
   
    else return false;
    
    return true;
}

void cFlatConfig::ThemeCheckAndInit(void) {
    if( strcmp(Theme.Name(), *ThemeCurrent) != 0 ) {
        dsyslog("skinflatplus: load theme: %s", *ThemeCurrent);
        ThemeCurrent = Theme.Name();
        ThemeInit();
    }
}

void cFlatConfig::DecorCheckAndInit(void) {
    if( DecorCurrent != DecorIndex ) {
        DecorCurrent = DecorIndex;
        DecorLoadCurrent();
    }
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

void cFlatConfig::ThemeInit(void) {
    decorBorderChannelFg = Theme.Color(clrChannelBorderFg);
    decorBorderChannelBg = Theme.Color(clrChannelBorderBg);

    decorBorderTopBarFg = Theme.Color(clrTopBarBorderFg);
    decorBorderTopBarBg = Theme.Color(clrTopBarBorderBg);

    decorBorderMessageFg = Theme.Color(clrMessageBorderFg);
    decorBorderMessageBg = Theme.Color(clrMessageBorderBg);

    decorBorderVolumeFg = Theme.Color(clrVolumeBorderFg);
    decorBorderVolumeBg = Theme.Color(clrVolumeBorderBg);

    decorBorderTrackFg = Theme.Color(clrTrackItemBorderFg);
    decorBorderTrackBg = Theme.Color(clrTrackItemBorderBg);
    decorBorderTrackCurFg = Theme.Color(clrTrackItemCurrentBorderFg);
    decorBorderTrackCurBg = Theme.Color(clrTrackItemCurrentBorderBg);

    decorBorderReplayFg = Theme.Color(clrReplayBorderFg);
    decorBorderReplayBg = Theme.Color(clrReplayBorderBg);

    decorBorderMenuItemFg = Theme.Color(clrMenuItemBorderFg);
    decorBorderMenuItemBg = Theme.Color(clrMenuItemBorderBg);
    decorBorderMenuItemSelFg = Theme.Color(clrMenuItemSelableBorderFg);
    decorBorderMenuItemSelBg = Theme.Color(clrMenuItemSelableBorderBg);
    decorBorderMenuItemCurFg = Theme.Color(clrMenuItemCurrentBorderFg);
    decorBorderMenuItemCurBg = Theme.Color(clrMenuItemCurrentBorderBg);

    decorBorderMenuContentHeadFg = Theme.Color(clrMenuContentHeadBorderFg);
    decorBorderMenuContentHeadBg = Theme.Color(clrMenuContentHeadBorderBg);

    decorBorderMenuContentFg = Theme.Color(clrMenuContentBorderFg);
    decorBorderMenuContentBg = Theme.Color(clrMenuContentBorderBg);

    decorBorderButtonFg = Theme.Color(clrButtonBorderFg);
    decorBorderButtonBg = Theme.Color(clrButtonBorderBg);
    
    decorProgressChannelFg = Theme.Color(clrChannelProgressFg);
    decorProgressChannelBarFg = Theme.Color(clrChannelProgressBarFg);
    decorProgressChannelBg = Theme.Color(clrChannelProgressBg);

    decorProgressVolumeFg = Theme.Color(clrVolumeProgressFg);
    decorProgressVolumeBarFg = Theme.Color(clrVolumeProgressBarFg);
    decorProgressVolumeBg = Theme.Color(clrVolumeProgressBg);

    decorProgressMenuItemFg = Theme.Color(clrMenuItemProgressFg);
    decorProgressMenuItemBarFg = Theme.Color(clrMenuItemProgressBarFg);
    decorProgressMenuItemBg = Theme.Color(clrMenuItemProgressBg);
    decorProgressMenuItemCurFg = Theme.Color(clrMenuItemCurProgressFg);
    decorProgressMenuItemCurBarFg = Theme.Color(clrMenuItemCurProgressBarFg);
    decorProgressMenuItemCurBg = Theme.Color(clrMenuItemCurProgressBg);

    decorProgressReplayFg = Theme.Color(clrReplayProgressFg);
    decorProgressReplayBarFg = Theme.Color(clrReplayProgressBarFg);
    decorProgressReplayBg = Theme.Color(clrReplayProgressBg);

    decorProgressSignalFg = Theme.Color(clrChannelSignalProgressFg);
    decorProgressSignalBarFg = Theme.Color(clrChannelSignalProgressBarFg);
    decorProgressSignalBg = Theme.Color(clrChannelSignalProgressBg);
}

void cFlatConfig::Init(void) {
    if( !strcmp(logoPath, "") ) {
        logoPath = cString::sprintf("%s/logos/", PLUGINRESOURCEPATH);
        dsyslog("skinflatplus: logoPath: %s", *logoPath);
    }
    if( !strcmp(iconPath, "") ) {
        iconPath = cString::sprintf("%s/icons/", PLUGINRESOURCEPATH);
        dsyslog("skinflatplus: iconPath: %s", *iconPath);
    }

    ThemeInit();
    DecorCheckAndInit();
}

bool stringCompare( const std::string &left, const std::string &right ) {
   for( std::string::const_iterator lit = left.begin(), rit = right.begin(); lit != left.end() && rit != right.end(); ++lit, ++rit )
      if( tolower( *lit ) < tolower( *rit ) )
         return true;
      else if( tolower( *lit ) > tolower( *rit ) )
         return false;
   if( left.size() < right.size() )
      return true;
   return false;
}

void cFlatConfig::DecorDescriptions(cStringList &Decors) {
    cString decorPath = cString::sprintf("%s/decors", PLUGINRESOURCEPATH);
    std::vector<std::string> files;
    Decors.Clear();
    
    cReadDir d(decorPath);
    struct dirent *e;
    while ((e = d.Next()) != NULL) {
        cString FileName = AddDirectory(decorPath, e->d_name);
        files.push_back(*FileName);
    }
    
    std::sort(files.begin(), files.end(), stringCompare);
    for (unsigned i = 0; i < files.size(); i++) {
        std::string FileName = files.at(i);
        cString Desc = DecorDescription(FileName.c_str());
        Decors.Append(strdup(*Desc));
    }
}

cString cFlatConfig::DecorDescription(cString File) {
    cString description;
    FILE *f = fopen(File, "r");
    if( f ) {
        int line = 0;
        char *s;
        cReadLine ReadLine;
        while( (s = ReadLine.Read(f)) != NULL ) {
            line++;
            char *p = strchr(s, '#');
            if (p)
                *p = 0;
            s = stripspace(skipspace(s));
            if (!isempty(s)) {
                char *n = s;
                char *v = strchr(s, '=');
                if (v) {
                    *v++ = 0;
                    n = stripspace(skipspace(n));
                    v = stripspace(skipspace(v));
                    if (strstr(n, "Description") == n) {
                        description = strdup(v);
                        break;
                    }
                }
            }
        }
    }
    return description;
}

void cFlatConfig::DecorLoadCurrent(void) {
    cString decorPath = cString::sprintf("%s/decors", PLUGINRESOURCEPATH);
    std::vector<std::string> files;
    
    cReadDir d(decorPath);
    struct dirent *e;
    while ((e = d.Next()) != NULL) {
        cString FileName = AddDirectory(decorPath, e->d_name);
        files.push_back(*FileName);
    }
    
    std::sort(files.begin(), files.end(), stringCompare);
    if( DecorIndex >= 0 && DecorIndex < (int)files.size() ) {
        std::string FileName = files.at(DecorIndex);
        DecorLoadFile( FileName.c_str() );
    }
}

void cFlatConfig::DecorLoadFile(cString File) {
    dsyslog("skinflatplus: load decor file: %s", *File);

    FILE *f = fopen(File, "r");
    if( f ) {
        int line = 0;
        char *s;
        cReadLine ReadLine;
        while( (s = ReadLine.Read(f)) != NULL ) {
            line++;
            char *p = strchr(s, '#');
            if (p)
                *p = 0;
            s = stripspace(skipspace(s));
            if (!isempty(s)) {
                char *n = s;
                char *v = strchr(s, '=');
                if (v) {
                    *v++ = 0;
                    n = stripspace(skipspace(n));
                    v = stripspace(skipspace(v));
                    int value = atoi( v );
                    if( strstr(n, "ChannelBorderType") == n ) {
                        decorBorderChannelTypeTheme = value; continue; }
                    if( strstr(n, "ChannelBorderSize") == n ) {
                        decorBorderChannelSizeTheme = value; continue; }
                    if( strstr(n, "TopBarBorderType") == n ) {
                        decorBorderTopBarTypeTheme = value; continue; }
                    if( strstr(n, "TopBarBorderSize") == n ) {
                        decorBorderTopBarSizeTheme = value; continue; }
                    if( strstr(n, "MessageBorderType") == n ) {
                        decorBorderMessageTypeTheme = value; continue; }
                    if( strstr(n, "MessageBorderSize") == n ) {
                        decorBorderMessageSizeTheme = value; continue; }
                    if( strstr(n, "VolumeBorderType") == n ) {
                        decorBorderVolumeTypeTheme = value; continue; }
                    if( strstr(n, "VolumeBorderSize") == n ) {
                        decorBorderVolumeSizeTheme = value; continue; }
                    if( strstr(n, "TrackItemBorderType") == n ) {
                        decorBorderTrackTypeTheme = value; continue; }
                    if( strstr(n, "TrackItemBorderSize") == n ) {
                        decorBorderTrackSizeTheme = value; continue; }
                    if( strstr(n, "ReplayBorderType") == n ) {
                        decorBorderReplayTypeTheme = value; continue; }
                    if( strstr(n, "ReplayBorderSize") == n ) {
                        decorBorderReplaySizeTheme = value; continue; }
                    if( strstr(n, "MenuItemBorderType") == n ) {
                        decorBorderMenuItemTypeTheme = value; continue; }
                    if( strstr(n, "MenuItemBorderSize") == n ) {
                        decorBorderMenuItemSizeTheme = value; continue; }
                    if( strstr(n, "MenuContentHeadBorderType") == n ) {
                        decorBorderMenuContentHeadTypeTheme = value; continue; }
                    if( strstr(n, "MenuContentHeadBorderSize") == n ) {
                        decorBorderMenuContentHeadSizeTheme = value; continue; }
                    if( strstr(n, "MenuContentBorderType") == n ) {
                        decorBorderMenuContentTypeTheme = value; continue; }
                    if( strstr(n, "MenuContentBorderSize") == n ) {
                        decorBorderMenuContentSizeTheme = value; continue; }
                    if( strstr(n, "ButtonBorderType") == n ) {
                        decorBorderButtonTypeTheme = value; continue; }
                    if( strstr(n, "ButtonBorderSize") == n ) {
                        decorBorderButtonSizeTheme = value; continue; }
                    if( strstr(n, "ChannelProgressType") == n ) {
                        decorProgressChannelTypeTheme = value; continue; }
                    if( strstr(n, "ChannelProgressSize") == n ) {
                        decorProgressChannelSizeTheme = value; continue; }
                    if( strstr(n, "VolumeProgressType") == n ) {
                        decorProgressVolumeTypeTheme = value; continue; }
                    if( strstr(n, "VolumeProgressSize") == n ) {
                        decorProgressVolumeSizeTheme = value; continue; }
                    if( strstr(n, "MenuItemProgressType") == n ) {
                        decorProgressMenuItemTypeTheme = value; continue; }
                    if( strstr(n, "MenuItemProgressSize") == n ) {
                        decorProgressMenuItemSizeTheme = value; continue; }
                    if( strstr(n, "ReplayProgressType") == n ) {
                        decorProgressReplayTypeTheme = value; continue; }
                    if( strstr(n, "ReplayProgressSize") == n ) {
                        decorProgressReplaySizeTheme = value; continue; }
                    if( strstr(n, "ChannelSignalProgressType") == n ) {
                        decorProgressSignalTypeTheme = value; continue; }
                    if( strstr(n, "ChannelSignalProgressSize") == n ) {
                        decorProgressSignalSizeTheme = value; continue; }
                }
            }
        }
    }
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
