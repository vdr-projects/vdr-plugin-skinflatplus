#include "displaymenu.h"

#ifndef VDRLOGO
    #define VDRLOGO "vdrlogo_default"
#endif

#include "symbols/1080/Cnew.xpm"
#include "symbols/1080/Carrowturn.xpm"
#include "symbols/1080/Crec.xpm"
#include "symbols/1080/Cclock.xpm"
#include "symbols/1080/Cclocksml.xpm"
#include "symbols/1080/Cvpssml.xpm"

cBitmap cFlatDisplayMenu::bmCNew(Cnew_xpm);
cBitmap cFlatDisplayMenu::bmCArrowTurn(Carrowturn_xpm);
cBitmap cFlatDisplayMenu::bmCRec(Crec_xpm);
cBitmap cFlatDisplayMenu::bmCClock(Cclock_xpm);
cBitmap cFlatDisplayMenu::bmCClocksml(Cclocksml_xpm);
cBitmap cFlatDisplayMenu::bmCVPS(Cvpssml_xpm);

/* Possible values of the stream content descriptor according to ETSI EN 300 468 */
enum stream_content
{
	sc_reserved       = 0x00,
	sc_video_MPEG2    = 0x01,
	sc_audio_MP2      = 0x02, // MPEG 1 Layer 2 audio
	sc_subtitle       = 0x03,
	sc_audio_AC3      = 0x04,
	sc_video_H264_AVC = 0x05,
	sc_audio_HEAAC    = 0x06,
};

cFlatDisplayMenu::cFlatDisplayMenu(void) {
    CreateFullOsd();
    TopBarCreate();
    ButtonsCreate();
    MessageCreate();

    VideoDiskUsageState = -1;
    
    itemHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    itemChannelHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    itemTimerHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    
    scrollBarWidth = ScrollBarWidth() + marginItem;
    scrollBarHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 + marginItem*3 + buttonsHeight + Config.decorBorderButtonSize*2);

    scrollBarTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2;
    isScrolling = false;
    ShowEvent = false;
    ShowRecording = false;
    ShowText = false;
    
    menuWidth = osdWidth;
    menuTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize;
    menuPixmap = osd->CreatePixmap(1, cRect(0, menuTop, menuWidth, scrollBarHeight ));
    menuIconsPixmap = osd->CreatePixmap(2, cRect(0, menuTop, menuWidth, scrollBarHeight ));
    
    chLeft = Config.decorBorderMenuContentHeadSize;
    chTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentHeadSize;
    chWidth = menuWidth - Config.decorBorderMenuContentHeadSize*2;
    chHeight = fontHeight + fontSmlHeight*2 + marginItem*2;
    contentHeadPixmap = osd->CreatePixmap(1, cRect(chLeft, chTop, chWidth, chHeight));
        
    scrollbarPixmap = osd->CreatePixmap(2, cRect(0, scrollBarTop, menuWidth, scrollBarHeight + buttonsHeight + Config.decorBorderButtonSize*2));

    menuPixmap->Fill(clrTransparent);
    menuIconsPixmap->Fill(clrTransparent);
    scrollbarPixmap->Fill(clrTransparent);
    
    menuCategory = mcUndefined;

    bmNew       = &bmCNew;
    bmRec       = &bmCRec;
    bmArrowTurn = &bmCArrowTurn;
    bmClock     = &bmCClock;
    bmClocksml  = &bmCClocksml;
    bmVPS       = &bmCVPS;
}

cFlatDisplayMenu::~cFlatDisplayMenu() {
    osd->DestroyPixmap(menuPixmap);
    osd->DestroyPixmap(menuIconsPixmap);
    osd->DestroyPixmap(scrollbarPixmap);
    osd->DestroyPixmap(contentHeadPixmap);
}

void cFlatDisplayMenu::SetMenuCategory(eMenuCategory MenuCategory) {
    ItemBorderClear();
    isScrolling = false;
    ShowRecording = ShowEvent = ShowText = false;
    
    menuCategory = MenuCategory;
    
    if( menuCategory == mcChannel ) {
        if( Config.MenuChannelView == 0 || Config.MenuChannelView == 1 || Config.MenuChannelView == 2 )
            itemChannelHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
        else if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 )
            itemChannelHeight = fontHeight + fontSmlHeight + marginItem + Config.decorProgressMenuItemSize + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    } else if( menuCategory == mcTimer ) {
        if( Config.MenuTimerView == 0 || Config.MenuTimerView == 1 )
            itemTimerHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
        else if( Config.MenuTimerView == 2 || Config.MenuTimerView == 3 )
            itemTimerHeight = fontHeight + fontSmlHeight + marginItem + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    } else if( menuCategory == mcSchedule || menuCategory == mcScheduleNow || menuCategory == mcScheduleNext ) {
        if( Config.MenuEventView == 0 || Config.MenuEventView == 1 )
            itemEventHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
        else if( Config.MenuEventView == 2 || Config.MenuEventView == 3 )
            itemEventHeight = fontHeight + fontSmlHeight + marginItem*2 + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2 + Config.decorProgressMenuItemSize/2;
    } else if( menuCategory == mcRecording ) {
        if( Config.MenuRecordingView == 0 || Config.MenuRecordingView == 1 )
            itemRecordingHeight = fontHeight + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
        else if( Config.MenuRecordingView == 2 || Config.MenuRecordingView == 3 )
            itemRecordingHeight = fontHeight + fontSmlHeight + marginItem + Config.MenuItemPadding + Config.decorBorderMenuItemSize*2;
    }
    
}

void cFlatDisplayMenu::DrawScrollbar(int Total, int Offset, int Shown, int Top, int Height, bool CanScrollUp, bool CanScrollDown, bool isContent) {
    if( Total > 0 && Total > Shown ) {
        if( isScrolling == false && ShowEvent == false && ShowRecording == false && ShowText == false ) {
            isScrolling = true;
            DecorBorderClearByFrom(BorderMenuItem);
            ItemBorderDrawAllWithScrollbar();
            ItemBorderClear();
            if( isContent )
                menuPixmap->DrawRectangle(cRect(menuItemWidth - scrollBarWidth + Config.decorBorderMenuContentSize, 0, scrollBarWidth + marginItem, scrollBarHeight), clrTransparent);
            else
                menuPixmap->DrawRectangle(cRect(menuItemWidth - scrollBarWidth + Config.decorBorderMenuItemSize, 0, scrollBarWidth + marginItem, scrollBarHeight), clrTransparent);
            
        }
    } else if( ShowEvent == false && ShowRecording == false && ShowText == false ) {
        isScrolling = false;
    }

    if( isContent )
        ScrollbarDraw(scrollbarPixmap, menuItemWidth - scrollBarWidth + Config.decorBorderMenuContentSize*2 + marginItem, Top, Height, Total, Offset, Shown, CanScrollUp, CanScrollDown);
    else
        ScrollbarDraw(scrollbarPixmap, menuItemWidth - scrollBarWidth + Config.decorBorderMenuItemSize*2 + marginItem, Top, Height, Total, Offset, Shown, CanScrollUp, CanScrollDown);
    
}

void cFlatDisplayMenu::SetScrollbar(int Total, int Offset) {
    DrawScrollbar(Total, Offset, MaxItems(), 0, ItemsHeight(), Offset > 0, Offset + MaxItems() < Total);
}

void cFlatDisplayMenu::Scroll(bool Up, bool Page) {
    // Wird das Menü gescrollt oder Content?
    if( ContentIsShown() ) {
        bool scrolled = ContentScroll(Up, Page);
        if( scrolled )
            DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal(), true);
    } else {
        cSkinDisplayMenu::Scroll(Up, Page);
    }
}

int cFlatDisplayMenu::MaxItems(void) {
    if( menuCategory == mcChannel )
        return scrollBarHeight / itemChannelHeight;
    else if( menuCategory == mcTimer )
        return scrollBarHeight / itemTimerHeight;
    else if( menuCategory == mcSchedule || menuCategory == mcScheduleNow || menuCategory == mcScheduleNext )
        return scrollBarHeight / itemEventHeight;
    else if( menuCategory == mcRecording )
        return scrollBarHeight / itemRecordingHeight;

    return scrollBarHeight / itemHeight;
}

int cFlatDisplayMenu::ItemsHeight(void) {
    if( menuCategory == mcChannel )
        return MaxItems() * itemChannelHeight - Config.MenuItemPadding;
    else if( menuCategory == mcTimer )
        return MaxItems() * itemTimerHeight - Config.MenuItemPadding;
    else if( menuCategory == mcSchedule || menuCategory == mcScheduleNow || menuCategory == mcScheduleNext )
        return MaxItems() * itemEventHeight - Config.MenuItemPadding;
    else if( menuCategory == mcRecording )
        return MaxItems() * itemRecordingHeight - Config.MenuItemPadding;

    return MaxItems() * itemHeight - Config.MenuItemPadding;
}

void cFlatDisplayMenu::Clear(void) {
    textScroller.Reset();
    menuPixmap->Fill(clrTransparent);
    menuIconsPixmap->Fill(clrTransparent);
    scrollbarPixmap->Fill(clrTransparent);
    contentHeadPixmap->Fill(clrTransparent);
    ContentClear();
    DecorBorderClearByFrom(BorderMenuItem);
    DecorBorderClearAll();
    isScrolling = false;

    ShowRecording = ShowEvent = ShowText = false;
}

void cFlatDisplayMenu::SetTitle(const char *Title) {
    if( (menuCategory == mcRecording || menuCategory == mcTimer) && Config.DiskUsageShow ) {
        cVideoDiskUsage::HasChanged(VideoDiskUsageState);
        int DiskUsage = cVideoDiskUsage::UsedPercent();
        double FreeGB = cVideoDiskUsage::FreeMB() / 1024.0;
        int FreeMinutes = cVideoDiskUsage::FreeMinutes();
        cString extra1 = cString::sprintf("%s: %d%%", tr("disk usage"), DiskUsage);
        cString extra2 = cString::sprintf("%s: %.1f GB ~ %02d:%02d", tr("free"), FreeGB, FreeMinutes / 60, FreeMinutes % 60);

        cString iconName("chart1");
        if( DiskUsage > 14 )
            iconName = "chart2";
        if( DiskUsage > 28 )
            iconName = "chart3";
        if( DiskUsage > 42 )
            iconName = "chart4";
        if( DiskUsage > 56 )
            iconName = "chart5";
        if( DiskUsage > 70 )
            iconName = "chart6";
        if( DiskUsage > 84 )
            iconName = "chart7";
        
        TopBarSetTitle(Title);
        TopBarSetTitleExtra(extra1, extra2);
        TopBarSetExtraIcon(iconName);
    } else {
        TopBarSetTitle(Title);
    }
    
    if( Config.TopBarMenuIconShow ) {
        cString icon;
        switch( menuCategory ) {
            case mcMain:
                TopBarSetTitle("");
                icon = cString::sprintf("menuIcons/%s", VDRLOGO);
                break;
            case mcSchedule:
            case mcScheduleNow:
            case mcScheduleNext:
                icon = "menuIcons/Schedule";
                break;
            case mcChannel:
                icon = "menuIcons/Channels";
                break;
            case mcTimer:
                icon = "menuIcons/Timers";
                break;
            case mcRecording:
                icon = "menuIcons/Recordings";
                break;
            case mcSetup:
                icon = "menuIcons/Setup";
                break;
            case mcCommand:
                icon = "menuIcons/Commands";
                break;
            case mcEvent:
                icon = "extraIcons/Info";
                break;
            case mcRecordingInfo:
                icon = "extraIcons/PlayInfo";
                break;
            default:
                icon = "";
        }
        TopBarSetMenuIcon(icon);
    }
}

void cFlatDisplayMenu::SetButtons(const char *Red, const char *Green, const char *Yellow, const char *Blue) {
    ButtonsSet(Red, Green, Yellow, Blue);
}

void cFlatDisplayMenu::SetMessage(eMessageType Type, const char *Text) {
    if (Text)
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cFlatDisplayMenu::SetItem(const char *Text, int Index, bool Current, bool Selectable) {
    ShowEvent = false;
    ShowRecording = false;
    ShowText = false;
    
    int y = Index * itemHeight;
    menuItemWidth = menuWidth - Config.decorBorderMenuItemSize*2;

    if( menuCategory == mcMain )
        menuItemWidth *= Config.MainMenuItemScale;
    
    int AvailableTextWidth = menuItemWidth - scrollBarWidth;
    if( isScrolling )
        menuItemWidth -= scrollBarWidth;

    tColor ColorFg, ColorBg;
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrItemCurrentBg);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrItemSelableBg);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrItemBg);
        }
    }
    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, menuItemWidth, fontHeight), ColorBg);
    int lh = fontHeight;
    int x1 = 0;
    int xOff = x1;
    for (int i = 0; i < MaxTabs; i++) {
        const char *s = GetTabbedText(Text, i);
        if( s ) {
            // from skinelchi
            char buffer[9];
            bool istimer = false;
            bool isnewrecording = false;
            bool hasEventtimer = false;
            bool haspartEventtimer = false;
            bool isRecording = false;
            bool hasVPS = false;
            bool isRunning = false;
            
            int xt = Tab(i);
            
            if( xt >= menuItemWidth )
                continue;
            if( true ) {
                // check if timer info symbols: " !#>"
                if (i == 0 && strlen(s) == 1 && strchr(" !#>", s[0])) {
                    istimer = true; // update status
                } else if (  (strlen(s) == 6 && s[5] == '*' && s[2] == ':' && isdigit(*s)
                    && isdigit(*(s + 1)) && isdigit(*(s + 3)) && isdigit(*(s + 4)))
                    || (strlen(s) == 5 && s[4] == '*' && s[1] == ':' && isdigit(*s)
                    && isdigit(*(s + 2)) && isdigit(*(s + 3)))
                    || (strlen(s) == 9 && s[8] == '*' && s[5] == '.' && s[2] == '.'
                    && isdigit(*s) && isdigit(*(s + 1)) && isdigit(*(s + 3))
                    && isdigit(*(s + 4)) && isdigit(*(s + 6)) && isdigit(*(s + 7)))) {
                    // check if new recording: "10:10*", "1:10*", "01.01.06*"
                    
                    isnewrecording = true;  // update status
                    strncpy(buffer, s, strlen(s));   // make a copy
                    buffer[strlen(s) - 1] = '\0';  // remove the '*' character
                } else if ( (strlen(s) == 3) && ( i == 2 || i == 3 || i == 4) ) {
                     if (s[0] == 'R') isRecording = true;
                     if (s[0] == 'T') hasEventtimer = true;
                     if (s[0] == 't') haspartEventtimer = true;
                     if (s[1] == 'V') hasVPS = true;
                     if (s[2] == '*') isRunning = true;
                } else if ( (strlen(s) == 4) && ( i == 3 ) ) { //epgsearch What's on now default
                        if (s[1] == 'R') isRecording = true;
                        if (s[1] == 'T') hasEventtimer = true;
                        if (s[1] == 't') haspartEventtimer = true;
                        if (s[2] == 'V') hasVPS = true;
                        if (s[3] == '*') isRunning = true;
                }
            }
            xOff = x1 + Tab(i) + Config.decorBorderMenuItemSize;
            
            if( istimer ) {
                // timer menu
                switch( s[0] ) {
                    case '!':
                       menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmArrowTurn->Height()) / 2), *bmArrowTurn, ColorFg, ColorBg);
                       break;
                    case '#':
                       menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmRec->Height()) / 2), *bmRec, ColorFg, ColorBg);
                       break;
                    case '>':
                       menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmClock->Height()) / 2), *bmClock, ColorFg, ColorBg);
                       break;
                    case ' ':
                    default:
                       break;
                }
            } else if( isRecording || hasEventtimer || haspartEventtimer || hasVPS || isRunning ) {
                // program schedule menu
                if( isRecording )
                   menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmRec->Height()) / 2), *bmRec, ColorFg, ColorBg);
                else {
                   if( hasEventtimer )
                      menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmClock->Height()) / 2), *bmClock, ColorFg, ColorBg);
                   if( haspartEventtimer )
                      menuPixmap->DrawBitmap(cPoint(xOff + (bmClock->Height() - bmClocksml->Height()) / 2, y + (lh - bmClocksml->Height()) / 2), *bmClocksml, ColorFg, ColorBg);
                }
                xOff += bmClock->Width(); // clock is wider than rec

                if( hasVPS )
                   menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmVPS->Height()) / 2), *bmVPS, ColorFg, ColorBg);
                xOff += bmVPS->Width();

                if( isRunning )
                    menuPixmap->DrawText(cPoint(xOff, y), "*", ColorFg, ColorBg, font, AvailableTextWidth - xOff);
            
            } else if( isnewrecording ) {
                // recordings menu
                menuPixmap->DrawText(cPoint(xOff, y), buffer, ColorFg, ColorBg, font, AvailableTextWidth - xOff);
                
                // draw symbol "new" centered
                int gap = std::max(0, (Tab(i+1)-Tab(i)- font->Width(buffer) - bmNew->Width()) / 2);
                menuPixmap->DrawBitmap(cPoint(xOff + font->Width(buffer) + gap, y + (lh - bmNew->Height()) / 2), *bmNew, ColorFg, ColorBg);
            } else if( CheckProgressBar( s ) ) {
                int colWidth = Tab(i+1) - Tab(i);

                tColor ColorFg = Config.decorProgressMenuItemFg;
                tColor ColorBarFg = Config.decorProgressMenuItemBarFg;
                tColor ColorBg = Config.decorProgressMenuItemBg;
                if( Current ) {
                    ColorFg = Config.decorProgressMenuItemCurFg;
                    ColorBarFg = Config.decorProgressMenuItemCurBarFg;
                    ColorBg = Config.decorProgressMenuItemCurBg;
                }
                cRect rec = cRect(xt + Config.decorBorderMenuItemSize,
                    y + (itemHeight-Config.MenuItemPadding)/2 - Config.decorProgressMenuItemSize/2 - Config.decorBorderMenuItemSize,
                    colWidth, Config.decorProgressMenuItemSize);
                cRect recBG = cRect(xt + Config.decorBorderMenuItemSize - marginItem, y,
                    colWidth + marginItem*2, fontHeight);

                DrawProgressBarFromText(rec, recBG, s, ColorFg, ColorBarFg, ColorBg);
            } else {
                if( (menuCategory == mcMain || menuCategory == mcSetup) && Config.MenuItemIconsShow) {
                    cString cIcon = GetIconName( MainMenuText(s) );
                    cImageLoader imgLoader;
                    cImage *img = imgLoader.LoadIcon(*cIcon, fontHeight - marginItem*2, fontHeight - marginItem*2);
                    if( img ) {
                        menuIconsPixmap->DrawImage(cPoint(xt + Config.decorBorderMenuItemSize + marginItem, y + marginItem), *img);
                    } else {
                        img = imgLoader.LoadIcon("menuIcons/blank", fontHeight - marginItem*2, fontHeight - marginItem*2);
                        if( img ) {
                            menuIconsPixmap->DrawImage(cPoint(xt + Config.decorBorderMenuItemSize + marginItem, y + marginItem), *img);
                        }
                    }
                    menuPixmap->DrawText(cPoint(fontHeight + marginItem*2 + xt + Config.decorBorderMenuItemSize, y), s, ColorFg, ColorBg, font,
                        AvailableTextWidth - xt - marginItem*2 - fontHeight);
                } else {
                    menuPixmap->DrawText(cPoint(xt + Config.decorBorderMenuItemSize, y), s, ColorFg, ColorBg, font);
                }
            }
        }
        if (!Tab(i + 1))
            break;
    }

    sDecorBorder ib;
    ib.Left = Config.decorBorderMenuItemSize;
    ib.Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize + y;
    
    ib.Width = menuWidth - Config.decorBorderMenuItemSize*2;
    if( isScrolling ) {
        ib.Width -= scrollBarWidth;
    }
    
    ib.Width = menuItemWidth;
    
    ib.Height = fontHeight;
    ib.Size = Config.decorBorderMenuItemSize;
    ib.Type = Config.decorBorderMenuItemType;

    if( Current ) {
        ib.ColorFg = Config.decorBorderMenuItemCurFg;
        ib.ColorBg = Config.decorBorderMenuItemCurBg;
    } else {
        if( Selectable ) {
            ib.ColorFg = Config.decorBorderMenuItemSelFg;
            ib.ColorBg = Config.decorBorderMenuItemSelBg;
        } else {
            ib.ColorFg = Config.decorBorderMenuItemFg;
            ib.ColorBg = Config.decorBorderMenuItemBg;
        }
    }

    DecorBorderDraw(ib.Left, ib.Top, ib.Width, ib.Height,
        ib.Size, ib.Type, ib.ColorFg, ib.ColorBg, BorderMenuItem);
    
    if( !isScrolling ) {
        ItemBorderInsertUnique(ib);
    }
    
    SetEditableWidth(menuWidth - Tab(1));
}

std::string cFlatDisplayMenu::items[16] = { "Schedule", "Channels", "Timers", "Recordings", "Setup", "Commands",
                                                "OSD", "EPG", "DVB", "LNB", "CAM", "Recording", "Replay", "Miscellaneous", "Plugins", "Restart"};

std::string cFlatDisplayMenu::MainMenuText(std::string Text) {
    std::string text = skipspace(Text.c_str());
    std::string menuEntry;
    std::string menuNumber;
    bool found = false;
    bool doBreak = false;
    size_t i = 0;
    for (; i < text.length(); i++) {
        char s = text.at(i);
        if (i==0) {
            //if text directly starts with nonnumeric, break
            if (!(s >= '0' && s <= '9')) {
                break;
            }
        }
        if (found) {
            if (!(s >= '0' && s <= '9')) {
                doBreak = true;
            }           
        }
        if (s >= '0' && s <= '9') {
            found = true;
        }
        if (doBreak)
            break;
        if (i>4)
            break;
    }
    if (found) {
        menuNumber = skipspace(text.substr(0,i).c_str());
        menuEntry = skipspace(text.substr(i).c_str());
    } else {
        menuNumber = "";
        menuEntry = text.c_str();       
    }
    return menuEntry;
}

cString cFlatDisplayMenu::GetIconName(std::string element) {
    //check for standard menu entries
    for (int i=0; i<16; i++) {
        std::string s = trVDR(items[i].c_str());
        if (s == element) {
            cString menuIcon = cString::sprintf("menuIcons/%s", items[i].c_str());
            return menuIcon;
        }
    }
    //check for special main menu entries "stop recording", "stop replay"
    std::string stopRecording = skipspace(trVDR(" Stop recording "));
    std::string stopReplay = skipspace(trVDR(" Stop replaying"));
    try {
        if (element.substr(0, stopRecording.size()) == stopRecording)
            return "menuIcons/StopRecording";
        if (element.substr(0, stopReplay.size()) == stopReplay)
            return "menuIcons/StopReplay";
    } catch (...) {}
    //check for Plugins
    for (int i = 0; ; i++) {
        cPlugin *p = cPluginManager::GetPlugin(i);
        if (p) {
            const char *mainMenuEntry = p->MainMenuEntry();
            if (mainMenuEntry) {
                std::string plugMainEntry = mainMenuEntry;
                try {
                    if (element.substr(0, plugMainEntry.size()) == plugMainEntry) {
                        return cString::sprintf("pluginIcons/%s", p->Name());
                    }
                } catch (...) {}
            } 
        } else
            break;
    }
    return cString::sprintf("extraIcons/%s", element.c_str());
}

bool cFlatDisplayMenu::CheckProgressBar(const char *text) {
    if (strlen(text) > 5 
        && text[0] == '['
        && ((text[1] == '|')||(text[1] == ' ')) 
        && ((text[2] == '|')||(text[2] == ' ')) 
        && text[strlen(text) - 1] == ']')
        return true;
    return false;
}

void cFlatDisplayMenu::DrawProgressBarFromText(cRect rec, cRect recBg, const char *bar, tColor ColorFg, tColor ColorBarFg, tColor ColorBg) {
    const char *p = bar + 1;
    bool isProgressbar = true;
    int total = 0;
    int now = 0;
    for (; *p != ']'; ++p) {
        if (*p == ' ' || *p == '|') {
            ++total;
            if (*p == '|')
                ++now;
        } else {
            isProgressbar = false;
            break;
        }
    }
    if (isProgressbar) {
        double progress = (double)now / (double)total;
        ProgressBarDrawRaw(menuPixmap, menuPixmap, rec, recBg, progress*total, total,
            ColorFg, ColorBarFg, ColorBg, Config.decorProgressMenuItemType, true);
    }
}

bool cFlatDisplayMenu::SetItemChannel(const cChannel *Channel, int Index, bool Current, bool Selectable, bool WithProvider) {
    if( Config.MenuChannelView == 0 || !Channel )
        return false;

    cSchedulesLock schedulesLock;
    const cSchedules *schedules = cSchedules::Schedules(schedulesLock);
    const cEvent *Event = NULL;
    
    bool DrawProgress = true;
    cString buffer;
    int y = Index * itemChannelHeight;

    int Height = fontHeight;
    if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 )
        Height = fontHeight + fontSmlHeight + marginItem + Config.decorProgressMenuItemSize;

    menuItemWidth = menuWidth - Config.decorBorderMenuItemSize*2;
    if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 )
        menuItemWidth *= 0.5;

    if( isScrolling )
        menuItemWidth -= scrollBarWidth;

    tColor ColorFg, ColorBg;
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrItemCurrentBg);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrItemSelableBg);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrItemBg);
        }
    }

    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, menuItemWidth, Height), ColorBg);
    
    int Left, Top, Width;
    int LeftName;
    Left = Config.decorBorderMenuItemSize + marginItem;
    Top = y;
    
    if( Channel->GroupSep() )
        DrawProgress = false;
    float progress = 0.0;
    cString EventTitle = "";
     
    cString ws = cString::sprintf("%d", Channels.MaxNumber());
    int w = font->Width(ws);
    if( !Channel->GroupSep() )
        buffer = cString::sprintf("%d", Channel->Number());
    else
        buffer = "";
    Width = font->Width(buffer);
    if( Width < w )
        Width = w;
    menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, Width, fontHeight, taRight);
    Left += Width + marginItem;
    
    int imageHeight = fontHeight;
    int imageLeft = Left;
    int imageTop = Top;
    cImage *img = imgLoader.LoadLogo(Channel->Name(), imageHeight, imageHeight);
    if( img ) {
        imageTop = Top + (fontHeight - img->Height()) / 2;
        menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
        Left += imageHeight + marginItem * 2;
    } else {
        bool isRadioChannel = ((!Channel->Vpid())&&(Channel->Apid(0))) ? true : false;

        if( isRadioChannel ) {
            img = imgLoader.LoadIcon("radio", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
                Left += imageHeight + marginItem * 2;
            }
        } else if( Channel->GroupSep() ) {
            img = imgLoader.LoadIcon("changroup", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
                Left += imageHeight + marginItem * 2;
            }
        } else {
            img = imgLoader.LoadIcon("tv", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
                Left += imageHeight + marginItem * 2;
            }
        }
    }

    LeftName = Left;
    
    // event from channel
    const cSchedule *Schedule = schedules->GetSchedule( Channel->GetChannelID() );
    if( Schedule ) {
        Event = Schedule->GetPresentEvent();
        if( Event ) {
            // calculate progress bar
            progress = (int)roundf( (float)(time(NULL) - Event->StartTime()) / (float) (Event->Duration()) * 100.0);
            if(progress < 0)
                progress = 0.;
            else if(progress > 100)
                progress = 100;

            EventTitle = Event->Title();
        }
    }
    
    if( WithProvider )
        buffer = cString::sprintf("%s - %s", Channel->Provider(), Channel->Name());
    else
        buffer = cString::sprintf("%s", Channel->Name());
    
    if( Config.MenuChannelView == 1 ) {
        Width = menuItemWidth - LeftName;
        menuPixmap->DrawText(cPoint(LeftName, Top), buffer, ColorFg, ColorBg, font, Width);
    } else {
        Width = menuItemWidth / 10*2;
        if( isScrolling )
            Width = (menuItemWidth + scrollBarWidth) / 10*2;
        
        if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 )
            Width = menuItemWidth - LeftName;
        
        menuPixmap->DrawText(cPoint(LeftName, Top), buffer, ColorFg, ColorBg, font, Width);
        
        Left += Width + marginItem;
        
        if( DrawProgress ) {
            int PBTop = y + (itemChannelHeight-Config.MenuItemPadding)/2 - Config.decorProgressMenuItemSize/2 - Config.decorBorderMenuItemSize;
            int PBLeft = Left;
            int PBWidth = menuItemWidth/10;
            if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 ) {
                PBTop = Top + fontHeight + fontSmlHeight;
                PBLeft = Left - Width - marginItem;
                PBWidth = menuItemWidth - LeftName - marginItem*2 - Config.decorBorderMenuItemSize - scrollBarWidth;
                
                if( isScrolling )
                    PBWidth += scrollBarWidth;
            }
            
            Width = menuItemWidth/10;
            if( isScrolling )
                Width = (menuItemWidth + scrollBarWidth) / 10;
            if( Current )
                ProgressBarDrawRaw(menuPixmap, menuPixmap, cRect( PBLeft, PBTop, PBWidth, Config.decorProgressMenuItemSize),
                    cRect( PBLeft, PBTop, PBWidth, Config.decorProgressMenuItemSize), progress, 100,
                    Config.decorProgressMenuItemCurFg, Config.decorProgressMenuItemCurBarFg, Config.decorProgressMenuItemCurBg, Config.decorProgressMenuItemType, false);
            else
                ProgressBarDrawRaw(menuPixmap, menuPixmap, cRect( PBLeft, PBTop, PBWidth, Config.decorProgressMenuItemSize),
                    cRect( PBLeft, PBTop, PBWidth, Config.decorProgressMenuItemSize), progress, 100,
                    Config.decorProgressMenuItemFg, Config.decorProgressMenuItemBarFg, Config.decorProgressMenuItemBg, Config.decorProgressMenuItemType, false);
            Left += Width + marginItem;
        }

        if( Config.MenuChannelView == 3 || Config.MenuChannelView == 4 ) {
            Left = LeftName;
            Top += fontHeight;
            menuPixmap->DrawText(cPoint(Left, Top), EventTitle, ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem );
        } else
            menuPixmap->DrawText(cPoint(Left, Top), EventTitle, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem );
    }
    
    sDecorBorder ib;
    ib.Left = Config.decorBorderMenuItemSize;
    ib.Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize + y;
    
    ib.Width = menuItemWidth - Config.decorBorderMenuItemSize*2;
    
    if( isScrolling ) {
        ib.Width -= scrollBarWidth;
    }
    
    ib.Width = menuItemWidth;
    
    ib.Height = Height;
    ib.Size = Config.decorBorderMenuItemSize;
    ib.Type = Config.decorBorderMenuItemType;

    if( Current ) {
        ib.ColorFg = Config.decorBorderMenuItemCurFg;
        ib.ColorBg = Config.decorBorderMenuItemCurBg;
    } else {
        if( Selectable ) {
            ib.ColorFg = Config.decorBorderMenuItemSelFg;
            ib.ColorBg = Config.decorBorderMenuItemSelBg;
        } else {
            ib.ColorFg = Config.decorBorderMenuItemFg;
            ib.ColorBg = Config.decorBorderMenuItemBg;
        }
    }

    DecorBorderDraw(ib.Left, ib.Top, ib.Width, ib.Height,
        ib.Size, ib.Type, ib.ColorFg, ib.ColorBg, BorderMenuItem);
    
    if( !isScrolling ) {
        ItemBorderInsertUnique(ib);
    }

    if( Config.MenuChannelView == 4 && Event && Current ) {
        DrawItemExtraEvent(Event, "");
    }
    
    return true;
}

void cFlatDisplayMenu::DrawItemExtraEvent(const cEvent *Event, cString EmptyText) {

    cLeft = menuItemWidth + Config.decorBorderMenuItemSize*2 + Config.decorBorderMenuContentSize + marginItem;
    if( isScrolling )
        cLeft += scrollBarWidth;
    cTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentSize;
    cWidth = menuWidth - cLeft - Config.decorBorderMenuContentSize;
    cHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 +
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3 + Config.decorBorderMenuContentSize*2);

    // Description
    ostringstream text;
    if( Event ) {
        if( !isempty(Event->Description()) ) {
            text << Event->Description();
        }
       
        if( Config.EpgAdditionalInfoShow ) {
            text << endl;
            const cComponents *Components = Event->Components();
            if (Components) {
                ostringstream audio;
                bool firstAudio = true;
                const char *audio_type = NULL;
                ostringstream subtitle;
                bool firstSubtitle = true;
                for (int i = 0; i < Components->NumComponents(); i++) {
                    const tComponent *p = Components->Component(i);
                    switch (p->stream) {
                        case sc_video_MPEG2:
                            if (p->description)
                                text << endl << tr("Video") << ": " <<  p->description << " (MPEG2)";
                            else
                                text << endl << tr("Video") << ": MPEG2";
                            break;
                        case sc_video_H264_AVC:
                            if (p->description)
                                text << endl << tr("Video") << ": " <<  p->description << " (H.264)";
                            else
                                text << endl << tr("Video") << ": H.264";
                            break;

                        case sc_audio_MP2:
                        case sc_audio_AC3:
                        case sc_audio_HEAAC:
                            if (firstAudio)
                                firstAudio = false;
                            else
                                audio << ", ";
                            switch (p->stream) {
                                case sc_audio_MP2:
                                    // workaround for wrongfully used stream type X 02 05 for AC3
                                    if (p->type == 5)
                                        audio_type = "AC3";
                                    else
                                        audio_type = "MP2";
                                    break;
                                case sc_audio_AC3:
                                    audio_type = "AC3"; break;
                                case sc_audio_HEAAC:
                                    audio_type = "HEAAC"; break;
                            }
                            if (p->description)
                                audio << p->description << " (" << audio_type << ", " << p->language << ")";
                            else
                                audio << p->language << " (" << audio_type << ")";
                            break;
                        case sc_subtitle:
                            if (firstSubtitle)
                                firstSubtitle = false;
                            else
                                subtitle << ", ";
                            if (p->description)
                                subtitle << p->description << " (" << p->language << ")";
                            else
                                subtitle << p->language;
                            break;
                    }
                }
                if (audio.str().length() > 0)
                    text << endl << tr("Audio") << ": "<< audio.str();
                if (subtitle.str().length() > 0)
                    text << endl << tr("Subtitle") << ": "<< subtitle.str();
            }
        }
    } else
        text << *EmptyText;
    ContentCreate(cLeft, cTop, cWidth, cHeight, 2);
    
    ContentSet( text.str().c_str(), Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );

    DecorBorderClearByFrom(BorderContent);
    if( Config.MenuContentFullSize )
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg, BorderContent);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg, BorderContent);
}

bool cFlatDisplayMenu::SetItemTimer(const cTimer *Timer, int Index, bool Current, bool Selectable) {
    if( Config.MenuTimerView == 0 || !Timer )
        return false;
    const cChannel *Channel = Timer->Channel();
    const cEvent *Event = Timer->Event();
    
    cString buffer;
    int y = Index * itemTimerHeight;

    int Height = fontHeight;
    if( Config.MenuTimerView == 2 || Config.MenuTimerView == 3 )
        Height = fontHeight + fontSmlHeight + marginItem;

    menuItemWidth = menuWidth - Config.decorBorderMenuItemSize*2;
    if( Config.MenuTimerView == 2 || Config.MenuTimerView == 3 )
        menuItemWidth *= 0.5;

    if( isScrolling )
        menuItemWidth -= scrollBarWidth;

    tColor ColorFg, ColorBg;
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrItemCurrentBg);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrItemSelableBg);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrItemBg);
        }
    }

    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, menuItemWidth, Height), ColorBg);
    cImage *img = NULL;
    int Left, Top;
    Left = Config.decorBorderMenuItemSize + marginItem;
    Top = y;

    int imageHeight = fontHeight;
    int imageLeft = Left;
    int imageTop = Top;
    
    cString TimerIconName("");
    if (!(Timer->HasFlags(tfActive))) {
        TimerIconName = "timerInactive";
        ColorFg = Theme.Color( clrMenuTimerItemDisabledFont );
    } else if (Timer->Recording()) {
        TimerIconName = "timerRecording";
        ColorFg = Theme.Color( clrMenuTimerItemRecordingFont );
    } else
        TimerIconName = "timerActive";
    
    img = imgLoader.LoadIcon(TimerIconName, imageHeight, imageHeight);
    if( img ) {
        imageTop = Top + (fontHeight - img->Height()) / 2;
        menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
        Left += imageHeight + marginItem * 2;
    }

    cString ws = cString::sprintf("%d", Channels.MaxNumber());
    int w = font->Width(ws);
    buffer = cString::sprintf("%d", Channel->Number());
    int Width = font->Width(buffer);
    if( Width < w )
        Width = w;
    menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, Width, fontHeight, taRight);
    Left += Width + marginItem;

    imageLeft = Left;
    
    img = imgLoader.LoadLogo(Channel->Name(), imageHeight, imageHeight);
    if( img ) {
        imageTop = Top + (fontHeight - img->Height()) / 2;
        menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
    } else {
        bool isRadioChannel = ((!Channel->Vpid())&&(Channel->Apid(0))) ? true : false;

        if( isRadioChannel ) {
            img = imgLoader.LoadIcon("radio", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
            }
        } else if( Channel->GroupSep() ) {
            img = imgLoader.LoadIcon("changroup", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
            }
        } else {
            img = imgLoader.LoadIcon("tv", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top + (fontHeight - img->Height()) / 2;
                menuIconsPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
            }
        }
    }
    Left += imageHeight + marginItem * 2;
    
    cString day, name("");
    if (Timer->WeekDays())
        day = Timer->PrintDay(0, Timer->WeekDays(), false);
    else if (Timer->Day() - time(NULL) < 28 * SECSINDAY) {
        day = itoa(Timer->GetMDay(Timer->Day()));
        name = WeekDayName(Timer->Day());
    } else {
        struct tm tm_r;
        time_t Day = Timer->Day();
        localtime_r(&Day, &tm_r);
        char buffer[16];
        strftime(buffer, sizeof(buffer), "%Y%m%d", &tm_r);
        day = buffer;
    }    
    const char *File = Setup.FoldersInTimerMenu ? NULL : strrchr(Timer->File(), FOLDERDELIMCHAR);
    if (File && strcmp(File + 1, TIMERMACRO_TITLE) && strcmp(File + 1, TIMERMACRO_EPISODE))
        File++;
    else 
        File = Timer->File();

    if( Config.MenuTimerView == 1 ) {
        buffer = cString::sprintf("%s%s%s.  %02d:%02d - %02d:%02d  %s",
                    *name, *name && **name ? " " : "", *day,
                    Timer->Start() / 100, Timer->Start() % 100,
                    Timer->Stop() / 100, Timer->Stop() % 100,
                    File);
        menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
    } else if( Config.MenuTimerView == 2 || Config.MenuTimerView == 3 ) {
        buffer = cString::sprintf("%s%s%s.  %02d:%02d - %02d:%02d",
                    *name, *name && **name ? " " : "", *day,
                    Timer->Start() / 100, Timer->Start() % 100,
                    Timer->Stop() / 100, Timer->Stop() % 100);
        menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
        menuPixmap->DrawText(cPoint(Left, Top + fontHeight), File, ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
    }
  
    sDecorBorder ib;
    ib.Left = Config.decorBorderMenuItemSize;
    ib.Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize + y;
    
    ib.Width = menuItemWidth - Config.decorBorderMenuItemSize*2;
    
    if( isScrolling ) {
        ib.Width -= scrollBarWidth;
    }
    
    ib.Width = menuItemWidth;
    
    ib.Height = Height;
    ib.Size = Config.decorBorderMenuItemSize;
    ib.Type = Config.decorBorderMenuItemType;

    if( Current ) {
        ib.ColorFg = Config.decorBorderMenuItemCurFg;
        ib.ColorBg = Config.decorBorderMenuItemCurBg;
    } else {
        if( Selectable ) {
            ib.ColorFg = Config.decorBorderMenuItemSelFg;
            ib.ColorBg = Config.decorBorderMenuItemSelBg;
        } else {
            ib.ColorFg = Config.decorBorderMenuItemFg;
            ib.ColorBg = Config.decorBorderMenuItemBg;
        }
    }

    DecorBorderDraw(ib.Left, ib.Top, ib.Width, ib.Height,
        ib.Size, ib.Type, ib.ColorFg, ib.ColorBg, BorderMenuItem);
    
    if( !isScrolling ) {
        ItemBorderInsertUnique(ib);
    }
    
    if( Config.MenuTimerView == 3 && Current ) {
        DrawItemExtraEvent(Event, tr("timer not enabled"));
    }
    
    return true;
}

bool cFlatDisplayMenu::SetItemEvent(const cEvent *Event, int Index, bool Current, bool Selectable, const cChannel *Channel, bool WithDate, eTimerMatch TimerMatch) {
    if( Config.MenuEventView == 0 )
        return false;
    
    cImage *img = NULL;
    cString buffer;
    int y = Index * itemEventHeight;

    int Height = fontHeight;
    if( Config.MenuEventView == 2 || Config.MenuEventView == 3 )
        Height = fontHeight + fontSmlHeight + marginItem*2 + Config.decorProgressMenuItemSize/2;

    menuItemWidth = menuWidth - Config.decorBorderMenuItemSize*2;
    if( Config.MenuEventView == 2 || Config.MenuEventView == 3 )
        menuItemWidth *= 0.6;

    if( isScrolling )
        menuItemWidth -= scrollBarWidth;

    tColor ColorFg, ColorBg, ColorShortTextFg;
    ColorShortTextFg = Theme.Color(clrMenuItemProgramShortTextFont);
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrItemCurrentBg);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrItemSelableBg);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrItemBg);
        }
    }

    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, menuItemWidth, Height), ColorBg);
    
    int Left = 0, Top = 0, LeftSecond = 0;
    LeftSecond = Left = Config.decorBorderMenuItemSize + marginItem;
    Top = y;
    int imageTop = Top;
    int w = 0;

    if( !Channel ) {
        int CurrentChannelNr = cDevice::CurrentChannel();
        cChannel *ChannelLogo = Channels.GetByNumber(CurrentChannelNr);
        
        cImage *img;
        img = imgLoader.LoadLogo(ChannelLogo->Name(), 999, topBarHeight - marginItem*2);
        if( img ) {
            TopBarSetMenuLogo( ChannelLogo->Name() );
        } else {
            bool isRadioChannel = ( (!ChannelLogo->Vpid()) && (ChannelLogo->Apid(0)) ) ? true : false;

            if( isRadioChannel ) {
                img = imgLoader.LoadIcon("radio", 999, topBarHeight - marginItem*2);
                if( img ) {
                    TopBarSetMenuLogo( ChannelLogo->Name() );
                }
            } else if( ChannelLogo->GroupSep() ) {
                img = imgLoader.LoadIcon("changroup", 999, topBarHeight - marginItem*2);
                if( img ) {
                    TopBarSetMenuIcon( ChannelLogo->Name() );
                }
            } else {
                img = imgLoader.LoadIcon("tv", 999, topBarHeight - marginItem*2);
                if( img ) {
                    TopBarSetMenuLogo( ChannelLogo->Name() );
                }
            }
        }
    }

    if( Channel ) {
        cString ws = cString::sprintf("%d", Channels.MaxNumber());
        w = font->Width(ws);
        if( !Channel->GroupSep() ) {
            buffer = cString::sprintf("%d", Channel->Number());
            int Width = font->Width(buffer);
            if( Width < w )
                Width = w;
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, Width, fontHeight, taRight);
        }
        Left += w + marginItem;
        
        img = imgLoader.LoadLogo(Channel->Name(), fontHeight, fontHeight);
        if( img ) {
            imageTop = Top + (fontHeight - img->Height()) / 2;
            menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
        } else {
            bool isRadioChannel = ((!Channel->Vpid())&&(Channel->Apid(0))) ? true : false;

            if( isRadioChannel ) {
                img = imgLoader.LoadIcon("radio", fontHeight, fontHeight);
                if( img ) {
                    imageTop = Top + (fontHeight - img->Height()) / 2;
                    menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
                }
            } else if( Channel->GroupSep() ) {
                img = imgLoader.LoadIcon("changroup", fontHeight, fontHeight);
                if( img ) {
                    imageTop = Top + (fontHeight - img->Height()) / 2;
                    menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
                }
            } else {
                img = imgLoader.LoadIcon("tv", fontHeight, fontHeight);
                if( img ) {
                    imageTop = Top + (fontHeight - img->Height()) / 2;
                    menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
                }
            }
        }
        
        Left += fontHeight + marginItem * 2;
        LeftSecond = Left;
        
        w = menuWidth / 10 * 2;
        menuPixmap->DrawText(cPoint(Left, Top), Channel->ShortName(true), ColorFg, ColorBg, font, w);
        Left += w + marginItem * 2;
        
        if( Event ) {
            int PBWidth = menuItemWidth/20;
            time_t now = time(NULL);
            if( (now >= Event->StartTime()) && (now <= Event->EndTime()) ) {
                int total = Event->EndTime() - Event->StartTime();
                if( total > 0 ) {
                    // calculate progress bar
                    double progress = (int)roundf( (float)(time(NULL) - Event->StartTime()) / (float) (Event->Duration()) * 100.0);
                    if(progress < 0)
                        progress = 0.;
                    else if(progress > 100)
                        progress = 100;
                    int PBTop =  y + (itemEventHeight - Config.MenuItemPadding)/2 - Config.decorProgressMenuItemSize/2 - Config.decorBorderMenuItemSize;
                    int PBLeft = Left;
                    int PBHeight = Config.decorProgressMenuItemSize;
                    
                    
                    if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) ) {
                        PBTop =  y + fontHeight + fontSmlHeight + marginItem;
                        PBWidth = menuItemWidth - LeftSecond - scrollBarWidth - marginItem * 2;
                        if( isScrolling )
                            PBWidth += scrollBarWidth;

                        PBLeft = LeftSecond;
                        PBHeight = Config.decorProgressMenuItemSize / 2;
                    }
                     
                    if( Current )
                        ProgressBarDrawRaw(menuPixmap, menuPixmap, cRect( PBLeft, PBTop, PBWidth, PBHeight),
                            cRect( PBLeft, PBTop, PBWidth, PBHeight), progress, 100,
                            Config.decorProgressMenuItemCurFg, Config.decorProgressMenuItemCurBarFg, Config.decorProgressMenuItemCurBg, Config.decorProgressMenuItemType, false);
                    else
                        ProgressBarDrawRaw(menuPixmap, menuPixmap, cRect( PBLeft, PBTop, PBWidth, PBHeight),
                            cRect( PBLeft, PBTop, PBWidth, PBHeight), progress, 100,
                            Config.decorProgressMenuItemFg, Config.decorProgressMenuItemBarFg, Config.decorProgressMenuItemBg, Config.decorProgressMenuItemType, false);
                }
            }
            Left += PBWidth + marginItem*2;
        }
    }

    if( WithDate && Event ) {
        if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) && Channel )
            w = fontSml->Width("XXX 99. ") + marginItem;
        else
            w = font->Width("XXX 99. ") + marginItem;
        
        struct tm tm_r;
        time_t Day = Event->StartTime();
        localtime_r(&Day, &tm_r);
        char buf[8];
        strftime(buf, sizeof(buf), "%2d", &tm_r);
        
        cString DateString = cString::sprintf("%s %s. ", *WeekDayName( (time_t)Event->StartTime()), buf );
        if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) && Channel ) {
            menuPixmap->DrawText(cPoint(LeftSecond, Top + fontHeight), DateString, ColorFg, ColorBg, fontSml, w);
            LeftSecond += w + marginItem;
        } else
            menuPixmap->DrawText(cPoint(Left, Top), DateString, ColorFg, ColorBg, font, w, fontHeight, taRight);
        
        Left += w + marginItem;
    }
    
    int imageHeight = fontHeight;
    if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) && Channel && Event ) {
        Top += fontHeight;
        Left = LeftSecond;
        imageHeight = fontSmlHeight;
        menuPixmap->DrawText(cPoint(Left, Top), Event->GetTimeString(), ColorFg, ColorBg, fontSml);
        Left += fontSml->Width( Event->GetTimeString() ) + marginItem;
    } else if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) && Event ){
        imageHeight = fontHeight;
        menuPixmap->DrawText(cPoint(Left, Top), Event->GetTimeString(), ColorFg, ColorBg, font);
        Left += font->Width( Event->GetTimeString() ) + marginItem;
    } else if( Event ){
        menuPixmap->DrawText(cPoint(Left, Top), Event->GetTimeString(), ColorFg, ColorBg, font);
        Left += font->Width( Event->GetTimeString() ) + marginItem;
    }
    
    if( TimerMatch == tmFull ) {
        img = imgLoader.LoadIcon("timer_full", imageHeight, imageHeight);
        if( img ) {
            imageTop = Top;
            menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
        }
    } else if( TimerMatch == tmPartial ) {
        img = imgLoader.LoadIcon("timer_partial", imageHeight, imageHeight);
        if( img ) {
            imageTop = Top;
            menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
        }
    }
    Left += imageHeight + marginItem;
    if( Event ) {
        if( Event->Vps() && (Event->Vps() - Event->StartTime()) ) {
            img = imgLoader.LoadIcon("vps", imageHeight, imageHeight);
            if( img ) {
                imageTop = Top;
                menuIconsPixmap->DrawImage( cPoint(Left, imageTop), *img );
            }
        } 
        Left += imageHeight + marginItem;

        if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) && Channel ) {
            menuPixmap->DrawText(cPoint(Left, Top), Event->Title(), ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
            if( Event->ShortText() ) {
                Left += fontSml->Width( Event->Title() );
                cString ShortText = cString::sprintf("  %s", Event->ShortText());
                menuPixmap->DrawText(cPoint(Left, Top), ShortText, ColorShortTextFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
            }
        } else if( (Config.MenuEventView == 2 || Config.MenuEventView == 3) ) {
            menuPixmap->DrawText(cPoint(Left, Top), Event->Title(), ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            if( Event->ShortText() ) {
                Top += fontHeight;
                menuPixmap->DrawText(cPoint(Left, Top), Event->ShortText(), ColorShortTextFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
            }
        } else {
            menuPixmap->DrawText(cPoint(Left, Top), Event->Title(), ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            if( Event->ShortText() ) {
                Left += font->Width(Event->Title());
                cString ShortText = cString::sprintf("  %s", Event->ShortText());
                menuPixmap->DrawText(cPoint(Left, Top), ShortText, ColorShortTextFg, ColorBg, font, menuItemWidth - Left - marginItem);
            }
        }
    }

    sDecorBorder ib;
    ib.Left = Config.decorBorderMenuItemSize;
    ib.Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize + y;
    
    ib.Width = menuItemWidth - Config.decorBorderMenuItemSize*2;
    
    if( isScrolling ) {
        ib.Width -= scrollBarWidth;
    }
    
    ib.Width = menuItemWidth;
    
    ib.Height = Height;
    ib.Size = Config.decorBorderMenuItemSize;
    ib.Type = Config.decorBorderMenuItemType;

    if( Current ) {
        ib.ColorFg = Config.decorBorderMenuItemCurFg;
        ib.ColorBg = Config.decorBorderMenuItemCurBg;
    } else {
        if( Selectable ) {
            ib.ColorFg = Config.decorBorderMenuItemSelFg;
            ib.ColorBg = Config.decorBorderMenuItemSelBg;
        } else {
            ib.ColorFg = Config.decorBorderMenuItemFg;
            ib.ColorBg = Config.decorBorderMenuItemBg;
        }
    }

    DecorBorderDraw(ib.Left, ib.Top, ib.Width, ib.Height,
        ib.Size, ib.Type, ib.ColorFg, ib.ColorBg, BorderMenuItem);
    
    if( !isScrolling ) {
        ItemBorderInsertUnique(ib);
    }

    if( Config.MenuEventView == 3 ) {
        DrawItemExtraEvent(Event, "");
    }
    
    return true;
}

const char * cFlatDisplayMenu::GetRecordingName(const cRecording *Recording, int Level, bool isFolder) {
    std::string recNamePart;
    std::string recName = Recording->Name();
    try {
        std::vector<std::string> tokens;
        std::istringstream f(recName.c_str());
        std::string s;
        while (std::getline(f, s, FOLDERDELIMCHAR)) {
            tokens.push_back(s);
        }
        recNamePart = tokens.at(Level);
        if(!isFolder && Recording->IsEdited() ) {
            recNamePart = recNamePart.substr(1);
        }
    } catch (...) {
        recNamePart = recName.c_str();
    }    
    return recNamePart.c_str();
}

bool cFlatDisplayMenu::SetItemRecording(const cRecording *Recording, int Index, bool Current, bool Selectable, int Level, int Total, int New) {
    if( Config.MenuRecordingView == 0 )
        return false;

    cString buffer;
    cString RecName = GetRecordingName(Recording, Level, Total == 0);
    int y = Index * itemRecordingHeight;

    int Height = fontHeight;
    if( Config.MenuRecordingView == 2 || Config.MenuRecordingView == 3 )
        Height = fontHeight + fontSmlHeight + marginItem;

    menuItemWidth = menuWidth - Config.decorBorderMenuItemSize*2;
    if( Config.MenuRecordingView == 2 || Config.MenuRecordingView == 3 )
        menuItemWidth *= 0.5;

    if( isScrolling )
        menuItemWidth -= scrollBarWidth;

    tColor ColorFg, ColorBg, ColorExtraTextFg;
    ColorExtraTextFg = Theme.Color(clrMenuItemReplayExtraTextFont);
    if (Current) {
        ColorFg = Theme.Color(clrItemCurrentFont);
        ColorBg = Theme.Color(clrItemCurrentBg);
    }
    else {
        if( Selectable ) {
            ColorFg = Theme.Color(clrItemSelableFont);
            ColorBg = Theme.Color(clrItemSelableBg);
        } else {
            ColorFg = Theme.Color(clrItemFont);
            ColorBg = Theme.Color(clrItemBg);
        }
    }

    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, menuItemWidth, Height), ColorBg);
    cImage *img = NULL;
    cImage *imgRecNew = imgLoader.LoadIcon("recording_new", fontHeight, fontHeight);
    cImage *imgRecNewSml = imgLoader.LoadIcon("recording_new", fontSmlHeight, fontSmlHeight);
    cImage *imgRecCut = imgLoader.LoadIcon("recording_cutted", fontHeight, fontHeight);

    int Left, Top;
    Left = Config.decorBorderMenuItemSize + marginItem;
    Top = y;

    if( Config.MenuRecordingView == 1 ) {
        int LeftWidth = Left + fontHeight + imgRecNew->Width() + imgRecCut->Width() + 
            marginItem * 3 + font->Width("99.99.99  99:99  99:99 ");

        if( Total == 0 ) {
            img = imgLoader.LoadIcon("recording", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += fontHeight + marginItem;
            }
            
            //int Minutes = max(0, (Recording->LengthInSeconds() + 30) / 60);
            int Minutes = (Recording->LengthInSeconds() + 30) / 60; 
            cString Length = cString::sprintf("%02d:%02d", Minutes / 60, Minutes % 60);
            buffer = cString::sprintf("%s  %s  %s ", *ShortDateString(Recording->Start()), *TimeString(Recording->Start()), *Length);

            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            
            Left += font->Width( buffer );
            if( Recording->IsNew() ) {
                if( imgRecNew ) {
                    menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecNew );
                }
            }
            Left += imgRecNew->Width() + marginItem;
            if (Recording->IsEdited()) {
                if( imgRecCut ) {
                    menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecCut );
                }
            }
            Left += imgRecCut->Width() + marginItem;

            menuPixmap->DrawText(cPoint(Left, Top), RecName, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
        } else if( Total > 0 ) {
            img = imgLoader.LoadIcon("folder", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += img->Width() + marginItem;
            }

            buffer = cString::sprintf("%d  ", Total);
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            Left += font->Width( buffer );

            if( imgRecNew )
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecNew );
            Left += imgRecNew->Width() + marginItem;
            buffer = cString::sprintf("%d", New);
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            Left += font->Width( buffer );
            
            menuPixmap->DrawText(cPoint(LeftWidth, Top), RecName, ColorFg, ColorBg, font, menuItemWidth - LeftWidth - marginItem);
            LeftWidth += font->Width(RecName) + marginItem*2;
            buffer = cString::sprintf("(%s)", *ShortDateString(Recording->Start()));
            menuPixmap->DrawText(cPoint(LeftWidth, Top), buffer, ColorExtraTextFg, ColorBg, font, menuItemWidth - LeftWidth - marginItem);
        } else if( Total == -1 ) {
            img = imgLoader.LoadIcon("folder", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += img->Width() + marginItem;
            }

            menuPixmap->DrawText(cPoint(Left, Top), Recording->FileName(), ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
        }
    } else {
        if( Total == 0 ) {
            img = imgLoader.LoadIcon("recording", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += fontHeight + marginItem;
            }
            int ImagesWidth = imgRecNew->Width() + imgRecCut->Width() + marginItem*2 + scrollBarWidth;
            if( isScrolling )
                ImagesWidth -= scrollBarWidth;

            menuPixmap->DrawText(cPoint(Left, Top), RecName, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem - ImagesWidth);
            Top += fontHeight;
            
            int Minutes = (Recording->LengthInSeconds() + 30) / 60; 
            cString Length = cString::sprintf("%02d:%02d", Minutes / 60, Minutes % 60);
            buffer = cString::sprintf("%s  %s  %s ", *ShortDateString(Recording->Start()), *TimeString(Recording->Start()), *Length);

            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
            
            Top -= fontHeight;
            Left = menuItemWidth - ImagesWidth;
            if( Recording->IsNew() ) {
                if( imgRecNew ) {
                    menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecNew );
                }
            }
            Left += imgRecNew->Width() + marginItem;
            if (Recording->IsEdited()) {
                if( imgRecCut ) {
                    menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecCut );
                }
            }
            Left += imgRecCut->Width() + marginItem;

        } else if( Total > 0 ) {
            img = imgLoader.LoadIcon("folder", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += img->Width() + marginItem;
            }
            menuPixmap->DrawText(cPoint(Left, Top), RecName, ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
            Left += font->Width(RecName) + marginItem*2;
            buffer = cString::sprintf("(%s)", *ShortDateString(Recording->Start()));
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorExtraTextFg, ColorBg, font, menuItemWidth - Left - marginItem);
            Left -= font->Width(RecName) + marginItem*2;

            Top += fontHeight;
            buffer = cString::sprintf("%d  ", Total);
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
            Left += fontSml->Width( buffer );

            if( imgRecNewSml )
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *imgRecNewSml );
            Left += imgRecNewSml->Width() + marginItem;
            buffer = cString::sprintf("%d", New);
            menuPixmap->DrawText(cPoint(Left, Top), buffer, ColorFg, ColorBg, fontSml, menuItemWidth - Left - marginItem);
        } else if( Total == -1 ) {
            img = imgLoader.LoadIcon("folder", fontHeight, fontHeight);
            if( img ) {
                menuIconsPixmap->DrawImage( cPoint(Left, Top), *img );
                Left += img->Width() + marginItem;
            }
            menuPixmap->DrawText(cPoint(Left, Top), Recording->FileName(), ColorFg, ColorBg, font, menuItemWidth - Left - marginItem);
        }
    }

    sDecorBorder ib;
    ib.Left = Config.decorBorderMenuItemSize;
    ib.Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuItemSize + y;
    
    ib.Width = menuItemWidth - Config.decorBorderMenuItemSize*2;
    
    if( isScrolling ) {
        ib.Width -= scrollBarWidth;
    }
    
    ib.Width = menuItemWidth;
    
    ib.Height = Height;
    ib.Size = Config.decorBorderMenuItemSize;
    ib.Type = Config.decorBorderMenuItemType;

    if( Current ) {
        ib.ColorFg = Config.decorBorderMenuItemCurFg;
        ib.ColorBg = Config.decorBorderMenuItemCurBg;
    } else {
        if( Selectable ) {
            ib.ColorFg = Config.decorBorderMenuItemSelFg;
            ib.ColorBg = Config.decorBorderMenuItemSelBg;
        } else {
            ib.ColorFg = Config.decorBorderMenuItemFg;
            ib.ColorBg = Config.decorBorderMenuItemBg;
        }
    }

    DecorBorderDraw(ib.Left, ib.Top, ib.Width, ib.Height,
        ib.Size, ib.Type, ib.ColorFg, ib.ColorBg, BorderMenuItem);
    
    if( !isScrolling ) {
        ItemBorderInsertUnique(ib);
    }
    
    if( Config.MenuRecordingView == 3 && Current ) {
        DrawItemExtraRecording(Recording, tr("no recording info"));
    }
    
    return true;
}

void cFlatDisplayMenu::SetEvent(const cEvent *Event) {
    if( !Event )
        return;

    ShowEvent = true;
    ShowRecording = false;
    ShowText = false;
    ItemBorderClear();

    cLeft = Config.decorBorderMenuContentSize;
    cTop = chTop + marginItem*3 + fontHeight + fontSmlHeight*2 +
        Config.decorBorderMenuContentSize + Config.decorBorderMenuContentHeadSize;
    cWidth = menuWidth - Config.decorBorderMenuContentSize*2;
    cHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 +
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3 + 
        chHeight + Config.decorBorderMenuContentHeadSize*2 + Config.decorBorderMenuContentSize*2);

    if( !ButtonsDrawn() )
        cHeight += buttonsHeight + Config.decorBorderButtonSize*2;

    menuItemWidth = cWidth;

    contentHeadPixmap->Fill(clrTransparent);
    contentHeadPixmap->DrawRectangle(cRect(0, 0, menuWidth, fontHeight + fontSmlHeight*2 + marginItem*2), Theme.Color(clrScrollbarBg));

    cString date = Event->GetDateString();
    cString startTime = Event->GetTimeString();
    cString endTime = Event->GetEndTimeString();

    cString timeString = cString::sprintf("%s %s - %s", *date, *startTime, *endTime);

    cString title = Event->Title();
    cString shortText = Event->ShortText();

    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem), timeString, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg), fontSml, menuWidth - marginItem*2);
    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem + fontSmlHeight), title, Theme.Color(clrMenuEventFontTitle), Theme.Color(clrMenuEventBg), font, menuWidth - marginItem*2);
    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem + fontSmlHeight + fontHeight), shortText, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg), fontSml, menuWidth - marginItem*2);

    DecorBorderDraw(chLeft, chTop, chWidth, chHeight, Config.decorBorderMenuContentHeadSize, Config.decorBorderMenuContentHeadType,
        Config.decorBorderMenuContentHeadFg, Config.decorBorderMenuContentHeadBg);
    
    // Description
    ostringstream text;
    if( !isempty(Event->Description()) ) {
        text << Event->Description();
    }
   
    if( Config.EpgAdditionalInfoShow ) {
        text << endl;
        const cComponents *Components = Event->Components();
        if (Components) {
            ostringstream audio;
            bool firstAudio = true;
            const char *audio_type = NULL;
            ostringstream subtitle;
            bool firstSubtitle = true;
            for (int i = 0; i < Components->NumComponents(); i++) {
                const tComponent *p = Components->Component(i);
                switch (p->stream) {
                    case sc_video_MPEG2:
                        if (p->description)
                            text << endl << tr("Video") << ": " <<  p->description << " (MPEG2)";
                        else
                            text << endl << tr("Video") << ": MPEG2";
                        break;
                    case sc_video_H264_AVC:
                        if (p->description)
                            text << endl << tr("Video") << ": " <<  p->description << " (H.264)";
                        else
                            text << endl << tr("Video") << ": H.264";
                        break;

                    case sc_audio_MP2:
                    case sc_audio_AC3:
                    case sc_audio_HEAAC:
                        if (firstAudio)
                            firstAudio = false;
                        else
                            audio << ", ";
                        switch (p->stream) {
                            case sc_audio_MP2:
                                // workaround for wrongfully used stream type X 02 05 for AC3
                                if (p->type == 5)
                                    audio_type = "AC3";
                                else
                                    audio_type = "MP2";
                                break;
                            case sc_audio_AC3:
                                audio_type = "AC3"; break;
                            case sc_audio_HEAAC:
                                audio_type = "HEAAC"; break;
                        }
                        if (p->description)
                            audio << p->description << " (" << audio_type << ", " << p->language << ")";
                        else
                            audio << p->language << " (" << audio_type << ")";
                        break;
                    case sc_subtitle:
                        if (firstSubtitle)
                            firstSubtitle = false;
                        else
                            subtitle << ", ";
                        if (p->description)
                            subtitle << p->description << " (" << p->language << ")";
                        else
                            subtitle << p->language;
                        break;
                }
            }
            if (audio.str().length() > 0)
                text << endl << tr("Audio") << ": "<< audio.str();
            if (subtitle.str().length() > 0)
                text << endl << tr("Subtitle") << ": "<< subtitle.str();
        }
    }
    
    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, text.str().c_str(), 0);
    if( contentScrollable ) {
        cWidth -= scrollBarWidth;
    }

    ContentCreate(cLeft, cTop, cWidth, cHeight, 0);
    
    ContentSet( text.str().c_str(), Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );
    if( ContentScrollable() ) {
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal(), true);
    }

    if( Config.MenuContentFullSize || ContentScrollable() )
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
}

void cFlatDisplayMenu::DrawItemExtraRecording(const cRecording *Recording, cString EmptyText) {
    cLeft = menuItemWidth + Config.decorBorderMenuItemSize*2 + Config.decorBorderMenuContentSize + marginItem;
    if( isScrolling )
        cLeft += scrollBarWidth;
    cTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentSize;
    cWidth = menuWidth - cLeft - Config.decorBorderMenuContentSize;
    cHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 +
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3 + Config.decorBorderMenuContentSize*2);

    ostringstream text;
    if( Recording ) {
        const cRecordingInfo *recInfo = Recording->Info();
        text.imbue(std::locale(""));

        if (!isempty(recInfo->Description()))
            text << recInfo->Description() << endl << endl;
        
        // lent from skinelchi
        if( Config.RecordingAdditionalInfoShow ) {
            cChannel *channel = Channels.GetByChannelID(((cRecordingInfo *)recInfo)->ChannelID());
            if (channel)
                text << trVDR("Channel") << ": " << channel->Number() << " - " << channel->Name() << endl;

            cMarks marks;
            bool hasMarks = marks.Load(Recording->FileName(), Recording->FramesPerSecond(), Recording->IsPesRecording()) && marks.Count();
            cIndexFile *index = new cIndexFile(Recording->FileName(), false, Recording->IsPesRecording());

            int lastIndex = 0;

            int cuttedLength = 0;
            long cutinframe = 0;
            unsigned long long recsize = 0;
            unsigned long long recsizecutted = 0;
            unsigned long long cutinoffset = 0;
            unsigned long long filesize[100000];
            filesize[0] = 0;

            int i = 0;
            int imax = 999;
            struct stat filebuf;
            cString filename;
            int rc = 0;

            do {
                if (Recording->IsPesRecording())
                    filename = cString::sprintf("%s/%03d.vdr", Recording->FileName(), ++i);
                else {
                    filename = cString::sprintf("%s/%05d.ts", Recording->FileName(), ++i);
                    imax = 99999;
                }
                rc=stat(filename, &filebuf);
                if (rc == 0)
                    filesize[i] = filesize[i-1] + filebuf.st_size;
                else {
                    if (ENOENT != errno) {
                        esyslog ("skinflatplus: error determining file size of \"%s\" %d (%s)", (const char *)filename, errno, strerror(errno));
                        recsize = 0;
                    }
                }
            } while( i <= imax && !rc );
            recsize = filesize[i-1];

            if (hasMarks && index) {
                uint16_t FileNumber;
                off_t FileOffset;

                bool cutin = true;
                cMark *mark = marks.First();
                while (mark) {
                    long position = mark->Position();
                    index->Get(position, &FileNumber, &FileOffset);
                    if (cutin) {
                        cutinframe = position;
                        cutin = false;
                        cutinoffset = filesize[FileNumber-1] + FileOffset;
                    } else {
                        cuttedLength += position - cutinframe;
                        cutin = true;
                        recsizecutted += filesize[FileNumber-1] + FileOffset - cutinoffset;
                    }
                    cMark *nextmark = marks.Next(mark);
                    mark = nextmark;
                }
                if( !cutin ) {
                    cuttedLength += index->Last() - cutinframe;
                    index->Get(index->Last() - 1, &FileNumber, &FileOffset);
                    recsizecutted += filesize[FileNumber-1] + FileOffset - cutinoffset;
                }
            }
            if (index) {
                lastIndex = index->Last();
                text << tr("Length") << ": " << *IndexToHMSF(lastIndex, false, Recording->FramesPerSecond());
                if (hasMarks)
                    text << " (" << tr("cutted") << ": " << *IndexToHMSF(cuttedLength, false, Recording->FramesPerSecond()) << ")";
                text << endl;
            }
            delete index;

            if (recsize > MEGABYTE(1023))
                text << tr("Size") << ": " << fixed << setprecision(2) << (float)recsize / MEGABYTE(1024) << " GB";
            else
                text << tr("Size") << ": " << recsize / MEGABYTE(1) << " MB";
            if( hasMarks )
                if (recsize > MEGABYTE(1023))
                    text << " (" <<  tr("cutted") << ": " << fixed << setprecision(2) <<  (float)recsizecutted/MEGABYTE(1024) << " GB)";
                else
                    text << " (" << tr("cutted") << ": " <<  recsizecutted/MEGABYTE(1) << " MB)";

            text << endl << trVDR("Priority") << ": " << Recording->Priority() << ", " << trVDR("Lifetime") << ": " << Recording->Lifetime() << endl;

            if( lastIndex ) {
                text << tr("format") << ": " << (Recording->IsPesRecording() ? "PES" : "TS") << ", " << tr("bit rate") << ": ~ " << fixed << setprecision (2) << (float)recsize/lastIndex*Recording->FramesPerSecond()*8/MEGABYTE(1) << " MBit/s (Video + Audio)";
            }
            const cComponents *Components = recInfo->Components();
            if( Components ) {
                ostringstream audio;
                bool firstAudio = true;
                const char *audio_type = NULL;
                ostringstream subtitle;
                bool firstSubtitle = true;
                for (int i = 0; i < Components->NumComponents(); i++) {
                    const tComponent *p = Components->Component(i);

                    switch (p->stream) {
                        case sc_video_MPEG2:
                            text << endl << tr("Video") << ": " <<  p->description << " (MPEG2)";
                            break;
                        case sc_video_H264_AVC:
                            text << endl << tr("Video") << ": " <<  p->description << " (H.264)";
                            break;
                        case sc_audio_MP2:
                        case sc_audio_AC3:
                        case sc_audio_HEAAC:
                            if (firstAudio)
                                firstAudio = false;
                            else
                                audio << ", ";
                            switch (p->stream) {
                                case sc_audio_MP2:
                                    // workaround for wrongfully used stream type X 02 05 for AC3
                                    if (p->type == 5)
                                        audio_type = "AC3";
                                    else
                                        audio_type = "MP2";
                                    break;
                                case sc_audio_AC3:
                                    audio_type = "AC3"; break;
                                case sc_audio_HEAAC:
                                    audio_type = "HEAAC"; break;
                            }
                            if (p->description)
                                audio << p->description << " (" << audio_type << ", " << p->language << ")";
                            else
                                audio << p->language << " (" << audio_type << ")";
                            break;
                        case sc_subtitle:
                            if (firstSubtitle)
                                firstSubtitle = false;
                            else
                                subtitle << ", ";
                            if (p->description)
                                subtitle << p->description << " (" << p->language << ")";
                            else
                                subtitle << p->language;
                            break;
                    }
                }
                if (audio.str().length() > 0)
                    text << endl << tr("Audio") << ": "<< audio.str();
                if (subtitle.str().length() > 0)
                    text << endl << tr("Subtitle") << ": "<< subtitle.str();
            }
            if (recInfo->Aux()) {
                string str_epgsearch = xml_substring(recInfo->Aux(), "<epgsearch>", "</epgsearch>");
                string channel, searchtimer, pattern;

                if (!str_epgsearch.empty()) {
                    channel = xml_substring(str_epgsearch, "<channel>", "</channel>");
                    searchtimer = xml_substring(str_epgsearch, "<searchtimer>", "</searchtimer>");
                    if (searchtimer.empty())
                        searchtimer = xml_substring(str_epgsearch, "<Search timer>", "</Search timer>");
                }

                string str_vdradmin = xml_substring(recInfo->Aux(), "<vdradmin-am>", "</vdradmin-am>");
                if (!str_vdradmin.empty()) {
                    pattern = xml_substring(str_vdradmin, "<pattern>", "</pattern>");
                }

                if ((!channel.empty() && !searchtimer.empty()) || !pattern.empty())  {
                    text << endl << endl << tr("additional information") << ":" << endl;
                    if (!channel.empty() && !searchtimer.empty()) {
                        text << "EPGsearch: " << tr("channel") << ": " << channel << ", " << tr("search pattern") << ": " << searchtimer;
                    }
                    if (!pattern.empty()) {
                        text << "VDRadmin-AM: " << tr("search pattern") << ": " << pattern;
                    }
                }
            }
        }
    } else
        text << *EmptyText;

    ContentCreate(cLeft, cTop, cWidth, cHeight, 2);
    
    ContentSet( text.str().c_str(), Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );

    DecorBorderClearByFrom(BorderContent);
    if( Config.MenuContentFullSize )
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg, BorderContent);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg, BorderContent);
}

void cFlatDisplayMenu::SetRecording(const cRecording *Recording) {
    if( !Recording )
        return;

    ShowEvent = false;
    ShowRecording = true;
    ShowText = false;
    ItemBorderClear();

    const cRecordingInfo *recInfo = Recording->Info();

    chLeft = Config.decorBorderMenuContentHeadSize;
    chTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentHeadSize;
    chWidth = menuWidth - Config.decorBorderMenuContentHeadSize*2;
    chHeight = fontHeight + fontSmlHeight*2 + marginItem*2;
    contentHeadPixmap = osd->CreatePixmap(1, cRect(chLeft, chTop, chWidth, chHeight));

    cLeft = Config.decorBorderMenuContentSize;
    cTop = chTop + marginItem*3 + fontHeight + fontSmlHeight*2 +
        Config.decorBorderMenuContentSize + Config.decorBorderMenuContentHeadSize;
    cWidth = menuWidth - Config.decorBorderMenuContentSize*2;
    cHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 +
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3 + 
        chHeight + Config.decorBorderMenuContentHeadSize*2 + Config.decorBorderMenuContentSize*2);

    if( !ButtonsDrawn() )
        cHeight += buttonsHeight + Config.decorBorderButtonSize*2;

    menuItemWidth = cWidth;

    ostringstream text;
    text.imbue(std::locale(""));

    if (!isempty(recInfo->Description()))
        text << recInfo->Description() << endl << endl;
    
    // lent from skinelchi
    if( Config.RecordingAdditionalInfoShow ) {
        cChannel *channel = Channels.GetByChannelID(((cRecordingInfo *)recInfo)->ChannelID());
        if (channel)
            text << trVDR("Channel") << ": " << channel->Number() << " - " << channel->Name() << endl;

        cMarks marks;
        bool hasMarks = marks.Load(Recording->FileName(), Recording->FramesPerSecond(), Recording->IsPesRecording()) && marks.Count();
        cIndexFile *index = new cIndexFile(Recording->FileName(), false, Recording->IsPesRecording());

        int lastIndex = 0;

        int cuttedLength = 0;
        long cutinframe = 0;
        unsigned long long recsize = 0;
        unsigned long long recsizecutted = 0;
        unsigned long long cutinoffset = 0;
        unsigned long long filesize[100000];
        filesize[0] = 0;

        int i = 0;
        int imax = 999;
        struct stat filebuf;
        cString filename;
        int rc = 0;

        do {
            if (Recording->IsPesRecording())
                filename = cString::sprintf("%s/%03d.vdr", Recording->FileName(), ++i);
            else {
                filename = cString::sprintf("%s/%05d.ts", Recording->FileName(), ++i);
                imax = 99999;
            }
            rc=stat(filename, &filebuf);
            if (rc == 0)
                filesize[i] = filesize[i-1] + filebuf.st_size;
            else {
                if (ENOENT != errno) {
                    esyslog ("skinflatplus: error determining file size of \"%s\" %d (%s)", (const char *)filename, errno, strerror(errno));
                    recsize = 0;
                }
            }
        } while( i <= imax && !rc );
        recsize = filesize[i-1];

        if (hasMarks && index) {
            uint16_t FileNumber;
            off_t FileOffset;

            bool cutin = true;
            cMark *mark = marks.First();
            while (mark) {
                long position = mark->Position();
                index->Get(position, &FileNumber, &FileOffset);
                if (cutin) {
                    cutinframe = position;
                    cutin = false;
                    cutinoffset = filesize[FileNumber-1] + FileOffset;
                } else {
                    cuttedLength += position - cutinframe;
                    cutin = true;
                    recsizecutted += filesize[FileNumber-1] + FileOffset - cutinoffset;
                }
                cMark *nextmark = marks.Next(mark);
                mark = nextmark;
            }
            if( !cutin ) {
                cuttedLength += index->Last() - cutinframe;
                index->Get(index->Last() - 1, &FileNumber, &FileOffset);
                recsizecutted += filesize[FileNumber-1] + FileOffset - cutinoffset;
            }
        }
        if (index) {
            lastIndex = index->Last();
            text << tr("Length") << ": " << *IndexToHMSF(lastIndex, false, Recording->FramesPerSecond());
            if (hasMarks)
                text << " (" << tr("cutted") << ": " << *IndexToHMSF(cuttedLength, false, Recording->FramesPerSecond()) << ")";
            text << endl;
        }
        delete index;

        if (recsize > MEGABYTE(1023))
            text << tr("Size") << ": " << fixed << setprecision(2) << (float)recsize / MEGABYTE(1024) << " GB";
        else
            text << tr("Size") << ": " << recsize / MEGABYTE(1) << " MB";
        if( hasMarks )
            if (recsize > MEGABYTE(1023))
                text << " (" <<  tr("cutted") << ": " << fixed << setprecision(2) <<  (float)recsizecutted/MEGABYTE(1024) << " GB)";
            else
                text << " (" << tr("cutted") << ": " <<  recsizecutted/MEGABYTE(1) << " MB)";

        text << endl << trVDR("Priority") << ": " << Recording->Priority() << ", " << trVDR("Lifetime") << ": " << Recording->Lifetime() << endl;

        if( lastIndex ) {
            text << tr("format") << ": " << (Recording->IsPesRecording() ? "PES" : "TS") << ", " << tr("bit rate") << ": ~ " << fixed << setprecision (2) << (float)recsize/lastIndex*Recording->FramesPerSecond()*8/MEGABYTE(1) << " MBit/s (Video + Audio)";
        }
        const cComponents *Components = recInfo->Components();
        if( Components ) {
            ostringstream audio;
            bool firstAudio = true;
            const char *audio_type = NULL;
            ostringstream subtitle;
            bool firstSubtitle = true;
            for (int i = 0; i < Components->NumComponents(); i++) {
                const tComponent *p = Components->Component(i);

                switch (p->stream) {
                    case sc_video_MPEG2:
                        text << endl << tr("Video") << ": " <<  p->description << " (MPEG2)";
                        break;
                    case sc_video_H264_AVC:
                        text << endl << tr("Video") << ": " <<  p->description << " (H.264)";
                        break;
                    case sc_audio_MP2:
                    case sc_audio_AC3:
                    case sc_audio_HEAAC:
                        if (firstAudio)
                            firstAudio = false;
                        else
                            audio << ", ";
                        switch (p->stream) {
                            case sc_audio_MP2:
                                // workaround for wrongfully used stream type X 02 05 for AC3
                                if (p->type == 5)
                                    audio_type = "AC3";
                                else
                                    audio_type = "MP2";
                                break;
                            case sc_audio_AC3:
                                audio_type = "AC3"; break;
                            case sc_audio_HEAAC:
                                audio_type = "HEAAC"; break;
                        }
                        if (p->description)
                            audio << p->description << " (" << audio_type << ", " << p->language << ")";
                        else
                            audio << p->language << " (" << audio_type << ")";
                        break;
                    case sc_subtitle:
                        if (firstSubtitle)
                            firstSubtitle = false;
                        else
                            subtitle << ", ";
                        if (p->description)
                            subtitle << p->description << " (" << p->language << ")";
                        else
                            subtitle << p->language;
                        break;
                }
            }
            if (audio.str().length() > 0)
                text << endl << tr("Audio") << ": "<< audio.str();
            if (subtitle.str().length() > 0)
                text << endl << tr("Subtitle") << ": "<< subtitle.str();
        }
        if (recInfo->Aux()) {
            string str_epgsearch = xml_substring(recInfo->Aux(), "<epgsearch>", "</epgsearch>");
            string channel, searchtimer, pattern;

            if (!str_epgsearch.empty()) {
                channel = xml_substring(str_epgsearch, "<channel>", "</channel>");
                searchtimer = xml_substring(str_epgsearch, "<searchtimer>", "</searchtimer>");
                if (searchtimer.empty())
                    searchtimer = xml_substring(str_epgsearch, "<Search timer>", "</Search timer>");
            }

            string str_vdradmin = xml_substring(recInfo->Aux(), "<vdradmin-am>", "</vdradmin-am>");
            if (!str_vdradmin.empty()) {
                pattern = xml_substring(str_vdradmin, "<pattern>", "</pattern>");
            }

            if ((!channel.empty() && !searchtimer.empty()) || !pattern.empty())  {
                text << endl << endl << tr("additional information") << ":" << endl;
                if (!channel.empty() && !searchtimer.empty()) {
                    text << "EPGsearch: " << tr("channel") << ": " << channel << ", " << tr("search pattern") << ": " << searchtimer;
                }
                if (!pattern.empty()) {
                    text << "VDRadmin-AM: " << tr("search pattern") << ": " << pattern;
                }
            }
        }
    }

    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, text.str().c_str(), 0);
    if( contentScrollable ) {
        cWidth -= scrollBarWidth;
    }

    ContentCreate(cLeft, cTop, cWidth, cHeight, 0);
    
    contentHeadPixmap->Fill(clrTransparent);
    contentHeadPixmap->DrawRectangle(cRect(0, 0, menuWidth, fontHeight + fontSmlHeight*2 + marginItem*2), Theme.Color(clrScrollbarBg));

    cString timeString = cString::sprintf("%s  %s  %s", *DateString(Recording->Start()), *TimeString(Recording->Start()), recInfo->ChannelName() ? recInfo->ChannelName() : "");

    cString title = recInfo->Title();
    if( isempty(title) )
        title = Recording->Name();
    cString shortText = recInfo->ShortText();

    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem), timeString, Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg), fontSml, menuWidth - marginItem*2);
    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem + fontSmlHeight), title, Theme.Color(clrMenuRecFontTitle), Theme.Color(clrMenuRecBg), font, menuWidth - marginItem*2);
    contentHeadPixmap->DrawText(cPoint(marginItem, marginItem + fontSmlHeight + fontHeight), shortText, Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg), fontSml, menuWidth - marginItem*2);

    DecorBorderDraw(chLeft, chTop, chWidth, chHeight, Config.decorBorderMenuContentHeadSize, Config.decorBorderMenuContentHeadType,
        Config.decorBorderMenuContentHeadFg, Config.decorBorderMenuContentHeadBg);
    
    ContentSet( text.str().c_str(), Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg) );
    if( ContentScrollable() ) {
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal(), true);
    }

    RecordingBorder.Left = cLeft;
    RecordingBorder.Top = cTop;
    RecordingBorder.Width = cWidth;
    RecordingBorder.Height = ContentGetHeight();
    RecordingBorder.Size = Config.decorBorderMenuContentSize;
    RecordingBorder.Type = Config.decorBorderMenuContentType;
    RecordingBorder.ColorFg = Config.decorBorderMenuContentFg;
    RecordingBorder.ColorBg = Config.decorBorderMenuContentBg;
    RecordingBorder.From = BorderMenuRecord;
    
    if( Config.MenuContentFullSize || ContentScrollable() )
        DecorBorderDraw(RecordingBorder.Left, RecordingBorder.Top, RecordingBorder.Width, RecordingBorder.Height,
            RecordingBorder.Size, RecordingBorder.Type,
            RecordingBorder.ColorFg, RecordingBorder.ColorBg, RecordingBorder.From);
    else
        DecorBorderDraw(RecordingBorder.Left, RecordingBorder.Top, RecordingBorder.Width, ContentGetTextHeight(),
            RecordingBorder.Size, RecordingBorder.Type,
            RecordingBorder.ColorFg, RecordingBorder.ColorBg, RecordingBorder.From);
}

void cFlatDisplayMenu::SetText(const char *Text, bool FixedFont) {
    if( !Text )
        return;

    ShowEvent = false;
    ShowRecording = false;
    ShowText = true;
    ItemBorderClear();

    contentHeadPixmap->Fill(clrTransparent);

    int Left = Config.decorBorderMenuContentSize;
    int Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentSize;
    int Width = menuWidth - Config.decorBorderMenuContentSize*2;
    int Height = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 +
        buttonsHeight + Config.decorBorderButtonSize*2 + Config.decorBorderMenuContentSize*2 + marginItem);

    if( !ButtonsDrawn() )
        Height += buttonsHeight + Config.decorBorderButtonSize*2;

    menuItemWidth = Width;

    bool contentScrollable = ContentWillItBeScrollable(Width, Height, Text, FixedFont);
    if( contentScrollable ) {
        Width -= scrollBarWidth;
    }

    if( FixedFont ) {
        ContentCreate(Left, Top, Width, Height, 1);
        ContentSet( Text, Theme.Color(clrMenuTextFixedFont), Theme.Color(clrMenuTextBg) );
    } else {
        ContentCreate(Left, Top, Width, Height, 1);
        ContentSet( Text, Theme.Color(clrMenuTextFont), Theme.Color(clrMenuTextBg) );
    }

    
    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal(), true);

    if( Config.MenuContentFullSize || ContentScrollable() )
        DecorBorderDraw(Left, Top, Width, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
    else
        DecorBorderDraw(Left, Top, Width, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
}

int cFlatDisplayMenu::GetTextAreaWidth(void) const {
    return menuWidth - (marginItem*2);
}

const cFont *cFlatDisplayMenu::GetTextAreaFont(bool FixedFont) const {
    const cFont *rfont = FixedFont ? fontFixed : font;
    return rfont;
}

void cFlatDisplayMenu::Flush(void) {
    TopBarUpdate();
    osd->Flush();
}

void cFlatDisplayMenu::ItemBorderInsertUnique(sDecorBorder ib) {
    std::list<sDecorBorder>::iterator it;
    for( it = ItemsBorder.begin(); it != ItemsBorder.end(); it++ ) {
        if( (*it).Left == ib.Left && (*it).Top == ib.Top ) {
            (*it).Left = ib.Left;
            (*it).Top = ib.Top;
            (*it).Width = ib.Width;
            (*it).Height = ib.Height;
            (*it).Size = ib.Size;
            (*it).Type = ib.Type;
            (*it).ColorFg = ib.ColorFg;
            (*it).ColorBg = ib.ColorBg;
            (*it).From = ib.From;
            return;
        }
    }
    
    ItemsBorder.push_back(ib);
}

void cFlatDisplayMenu::ItemBorderDrawAllWithScrollbar(void) {
    std::list<sDecorBorder>::iterator it;
    for( it = ItemsBorder.begin(); it != ItemsBorder.end(); it++ ) {
        DecorBorderDraw((*it).Left, (*it).Top, (*it).Width - scrollBarWidth, (*it).Height, (*it).Size, (*it).Type,
            (*it).ColorFg, (*it).ColorBg, BorderMenuItem);
    }
}

void cFlatDisplayMenu::ItemBorderDrawAllWithoutScrollbar(void) {
    std::list<sDecorBorder>::iterator it;
    for( it = ItemsBorder.begin(); it != ItemsBorder.end(); it++ ) {
        DecorBorderDraw((*it).Left, (*it).Top, (*it).Width + scrollBarWidth, (*it).Height, (*it).Size, (*it).Type,
            (*it).ColorFg, (*it).ColorBg, BorderMenuItem);
    }
}

void cFlatDisplayMenu::ItemBorderClear(void) {
    ItemsBorder.clear();
}

// returns the string between start and end or an empty string if not found
string cFlatDisplayMenu::xml_substring(string source, const char* str_start, const char* str_end) {
    size_t start = source.find(str_start);
    size_t end   = source.find(str_end);

    if (string::npos != start && string::npos != end) {
        return (source.substr(start + strlen(str_start), end - start - strlen(str_start)));
    }

    return string();
}
