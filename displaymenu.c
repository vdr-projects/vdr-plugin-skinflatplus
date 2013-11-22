#include "displaymenu.h"

#include <ctype.h>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

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
    
    scrollBarWidth = ScrollBarWidth() + Config.MenuItemPadding;
    scrollBarHeight = osdHeight - (topBarHeight + Config.decorBorderTopBarSize*2 + 
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3 );
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
    
    scrollbarPixmap = osd->CreatePixmap(2, cRect(osdWidth - scrollBarWidth, scrollBarTop, scrollBarWidth, scrollBarHeight));

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
    menuCategory = MenuCategory;
}

void cFlatDisplayMenu::DrawScrollbar(int Total, int Offset, int Shown, int Top, int Height, bool CanScrollUp, bool CanScrollDown) {
    if( Total > 0 && Total > Shown ) {
        if( isScrolling == false && ShowEvent == false && ShowRecording == false && ShowText == false ) {
            isScrolling = true;
            DecorBorderClearByFrom(BorderMenuItem);
            ItemBorderDrawAllWithScrollbar();
            ItemBorderClear();
            menuPixmap->DrawRectangle(cRect(menuWidth - scrollBarWidth - Config.decorBorderMenuItemSize, 0, scrollBarWidth, scrollBarHeight), clrTransparent);
        }
    } else if( ShowEvent == false && ShowRecording == false && ShowText == false ) {
        isScrolling = false;
    }

    ScrollbarDraw(scrollbarPixmap, Config.MenuItemPadding, Top, Height, Total, Offset, Shown, CanScrollUp, CanScrollDown);
}

void cFlatDisplayMenu::SetScrollbar(int Total, int Offset) {
    DrawScrollbar(Total, Offset, MaxItems(), 0, ItemsHeight(), Offset > 0, Offset + MaxItems() < Total);
}

void cFlatDisplayMenu::Scroll(bool Up, bool Page) {
    // Wird das Menü gescrollt oder Content?
    if( ContentIsShown() )
    {
        bool scrolled = ContentScroll(Up, Page);
        if( scrolled )
        {
            DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());
        }
    } else {
        cSkinDisplayMenu::Scroll(Up, Page);
    }
}

int cFlatDisplayMenu::MaxItems(void) {
    if( menuCategory == mcChannel )
        return scrollBarHeight / itemChannelHeight;

    return scrollBarHeight / itemHeight;
}

int cFlatDisplayMenu::ItemsHeight(void) {
    if( menuCategory == mcChannel )
        return MaxItems() * itemChannelHeight - Config.MenuItemPadding;

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
                icon = "menuIcons/vdrlogo";
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
    int Width = menuWidth - Config.decorBorderMenuItemSize*2;
    if( isScrolling )
        Width -= scrollBarWidth;
    
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
    menuPixmap->DrawRectangle(cRect(Config.decorBorderMenuItemSize, y, Width, fontHeight), ColorBg);
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
            } else if (isRecording || hasEventtimer || haspartEventtimer || hasVPS || isRunning) {
                // program schedule menu
                if (isRecording)
                   menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmRec->Height()) / 2), *bmRec, ColorFg, ColorBg);
                else {
                   if (hasEventtimer)
                      menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmClock->Height()) / 2), *bmClock, ColorFg, ColorBg);
                   if (haspartEventtimer)
                      menuPixmap->DrawBitmap(cPoint(xOff + (bmClock->Height() - bmClocksml->Height()) / 2, y + (lh - bmClocksml->Height()) / 2), *bmClocksml, ColorFg, ColorBg);
                }
                xOff += bmClock->Width(); // clock is wider than rec

                if (hasVPS)
                   menuPixmap->DrawBitmap(cPoint(xOff, y + (lh - bmVPS->Height()) / 2), *bmVPS, ColorFg, ColorBg);
                xOff += bmVPS->Width();

                if( isRunning )
                    menuPixmap->DrawText(cPoint(xOff, y), "*", ColorFg, ColorBg, font, xOff);
            
            } else if (isnewrecording) {
                // recordings menu
                menuPixmap->DrawText(cPoint(xOff, y), buffer, ColorFg, ColorBg, font, xOff);
                
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
                DrawProgressBarFromText(y + (itemHeight-Config.MenuItemPadding)/2 - Config.decorProgressMenuItemSize/2 - Config.decorBorderMenuItemSize,
                    xt + Config.decorBorderMenuItemSize, colWidth, s, ColorFg, ColorBarFg, ColorBg);
            } else {
                if( (menuCategory == mcMain || menuCategory == mcSetup) && Config.MenuItemIconsShow) {

                    cString cIcon = GetIconName( MainMenuText(s) );
                    cImageLoader imgLoader;
                    if (imgLoader.LoadIcon(*cIcon, fontHeight -marginItem*2)) {
                        menuIconsPixmap->DrawImage(cPoint(xt + Config.decorBorderMenuItemSize + marginItem, y + marginItem), imgLoader.GetImage());
                    } else {
                        if (imgLoader.LoadIcon("menuIcons/blank", fontHeight)) {
                            menuIconsPixmap->DrawImage(cPoint(xt + Config.decorBorderMenuItemSize + marginItem, y + marginItem), imgLoader.GetImage());
                        }
                    }
                    menuPixmap->DrawText(cPoint(fontHeight + marginItem*2 + xt + Config.decorBorderMenuItemSize, y), s, ColorFg, ColorBg, font,
                        Width - xt - marginItem*2 - fontHeight );
                } else {
                    menuPixmap->DrawText(cPoint(xt + Config.decorBorderMenuItemSize, y), s, ColorFg, ColorBg, font,
                        Width - xt);
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

void cFlatDisplayMenu::DrawProgressBarFromText(int Top, int Left, int Width, const char *bar, tColor ColorFg, tColor ColorBarFg, tColor ColorBg) {
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
        double progress = (double)now/(double)total;
        ProgressBarDrawRaw(menuPixmap, menuPixmap, cRect(Left, Top, Width, Config.decorProgressMenuItemSize),
            cRect(Left, Top, Width, Config.decorProgressMenuItemSize), progress*total, total,
            ColorFg, ColorBarFg, ColorBg, Config.decorProgressMenuItemType);
    }
}

/*
bool cFlatDisplayMenu::SetItemChannel(const cChannel *Channel, int Index, bool Current, bool Selectable, bool WithProvider) {
    cSchedulesLock schedulesLock;
    const cSchedules *schedules = cSchedules::Schedules(schedulesLock);

    cString buffer;
    int y = Index * itemChannelHeight;
    
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
    menuPixmap->DrawRectangle(cRect(0, y, menuWidth, itemChannelHeight - paddingMenuItem), ColorBg);
    
    // event from channel
    const cSchedule *Schedule = schedules->GetSchedule( Channel->GetChannelID() );
    if( Schedule ) {
        const cEvent *Event = Schedule->GetPresentEvent();
        if( Event ) {
            // calculate progress bar
            float progress = (int)roundf( (float)(time(NULL) - Event->StartTime()) / (float) (Event->Duration()) * 100.0);
            if(progress < 0)
                progress = 0.;
            else if(progress > 100)
                progress = 100;

            if( WithProvider )
                buffer = cString::sprintf("%d\t%s - %s", Channel->Number(), Channel->Provider(), Channel->Name());
            else
                buffer = cString::sprintf("%d\t%s", Channel->Number(), Channel->Name());

            const char *s1 = GetTabbedText(buffer, 0);
            if( s1 ) {
                int xt = Tab(0);
                menuPixmap->DrawText(cPoint(marginItem + xt, y), s1, ColorFg, ColorBg, font);
            }
            const char *s2 = GetTabbedText(buffer, 1);
            if( s2 ) {
                int xt = Tab(1);
                int w = (menuWidth / 10 * 3) - marginItem;
                menuPixmap->DrawText(cPoint(marginItem + xt, y), s2, ColorFg, ColorBg, font, w);
            }
            
            menuPixmap->DrawRectangle(cRect( (menuWidth/10*3) + marginItem, y, marginItem, fontHeight), ColorBg);
            
            if( Current )
                ProgressBarDrawInline(menuPixmap, (menuWidth/10*3) + marginItem*2, y, menuWidth/10 - marginItem, fontHeight,
                    progress, 100, Theme.Color(clrMenuItemChanCurProgressFg), Theme.Color(clrMenuItemChanCurProgressBarFg),
                    Theme.Color(clrMenuItemChanCurProgressBg));
            else
                ProgressBarDrawInline(menuPixmap, (menuWidth/10*3) + marginItem*2, y, menuWidth/10 - marginItem, fontHeight,
                    progress, 100, Theme.Color(clrMenuItemChanProgressFg), Theme.Color(clrMenuItemChanProgressBarFg),
                    Theme.Color(clrMenuItemChanProgressBg));
            menuPixmap->DrawText(cPoint((menuWidth / 10 * 4) + marginItem*2, y), Event->Title(), ColorFg, ColorBg, font);
            
            return true;
        }
    }

    // without schedule, do it like vdr
    if (!Channel->GroupSep()) {
        if( WithProvider )
            buffer = cString::sprintf("%d\t%s - %s", Channel->Number(), Channel->Provider(), Channel->Name());
        else
            buffer = cString::sprintf("%d\t%s", Channel->Number(), Channel->Name());

        const char *s1 = GetTabbedText(buffer, 0);
        if( s1 ) {
            int xt = Tab(0);
            menuPixmap->DrawText(cPoint(marginItem + xt, y), s1, ColorFg, ColorBg, font);
        }
        const char *s2 = GetTabbedText(buffer, 1);
        if( s2 ) {
            int xt = Tab(1);
            int w = (menuWidth / 10 * 3) - marginItem;

            menuPixmap->DrawText(cPoint(marginItem + xt, y), s2, ColorFg, ColorBg, font, w);
        }
    }
    else {
        buffer = cString::sprintf("---%s ----------------------------------------------------------------", Channel->Name());
        menuPixmap->DrawText(cPoint(marginItem, y), buffer, ColorFg, ColorBg, font);
    }

    return true;
}
*/

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
    
    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, text.str().c_str(), false);
    if( contentScrollable ) {
        cWidth -= scrollBarWidth;
    }

    ContentCreate(cLeft, cTop, cWidth, cHeight, false);
    
    ContentSet( text.str().c_str(), false, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );
    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());

    if( Config.MenuContentFullSize || ContentScrollable() )
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
}

// returns the string between start and end or an empty string if not found
string xml_substring(string source, const char* str_start, const char* str_end) {
    size_t start = source.find(str_start);
    size_t end   = source.find(str_end);

    if (string::npos != start && string::npos != end) {
        return (source.substr(start + strlen(str_start), end - start - strlen(str_start)));
    }

    return string();
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
                    esyslog ("skinelchi: error determining file size of \"%s\" %d (%s)", (const char *)filename, errno, strerror(errno));
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

    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, text.str().c_str(), false);
    if( contentScrollable ) {
        cWidth -= scrollBarWidth;
    }

    ContentCreate(cLeft, cTop, cWidth, cHeight, false);
    
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
    
    ContentSet( text.str().c_str(), false, Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg) );
    if( ContentScrollable() ) {
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());
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
        buttonsHeight + Config.decorBorderButtonSize*2 + marginItem*3);

    bool contentScrollable = ContentWillItBeScrollable(Width, Height, Text, FixedFont);
    if( contentScrollable ) {
        Width -= scrollBarWidth;
    }

    ContentCreate(Left, Top, Width, Height, FixedFont);

    ContentSet( Text, FixedFont, Theme.Color(clrMenuTextFont), Theme.Color(clrMenuTextBg) );
    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());

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