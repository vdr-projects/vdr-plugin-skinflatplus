#include "displaymenu.h"

#include "symbols/720/Bvpssml.xpm"
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
cBitmap cFlatDisplayMenu::bmCVPS(Bvpssml_xpm);

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
    
    chLeft = Config.decorBorderMenuContentHeadSize;
    chTop = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentHeadSize;
    chWidth = menuWidth - Config.decorBorderMenuContentHeadSize*2;
    chHeight = fontHeight + fontSmlHeight*2 + marginItem*2;
    contentHeadPixmap = osd->CreatePixmap(1, cRect(chLeft, chTop, chWidth, chHeight));
    
    scrollbarPixmap = osd->CreatePixmap(2, cRect(osdWidth - scrollBarWidth, scrollBarTop, scrollBarWidth, scrollBarHeight));

    menuPixmap->Fill(clrTransparent);
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
    scrollbarPixmap->Fill(clrTransparent);
    contentHeadPixmap->Fill(clrTransparent);
    ContentClear();
    DecorBorderClearAll();
    isScrolling = false;
    // DecorBorderClearByFrom( BorderMenuItem );
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
    } else
        TopBarSetTitle(Title);
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
                menuPixmap->DrawText(cPoint(xt + Config.decorBorderMenuItemSize, y), s, ColorFg, ColorBg, font,
                    Width - xt);
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

    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, Event->Description(), false);
    if( contentScrollable ) {
        cWidth -= scrollBarWidth;
    }

    ContentCreate(cLeft, cTop, cWidth, cHeight, false);
    
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
    
    ContentSet( Event->Description(), false, Theme.Color(clrMenuEventFontInfo), Theme.Color(clrMenuEventBg) );
    if( ContentScrollable() )
        DrawScrollbar(ContentScrollTotal(), ContentScrollOffset(), ContentVisibleLines(), contentTop - scrollBarTop, ContentGetHeight(), ContentScrollOffset() > 0, ContentScrollOffset() + ContentVisibleLines() < ContentScrollTotal());

    if( Config.MenuContentFullSize || ContentScrollable() )
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
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

    bool contentScrollable = ContentWillItBeScrollable(cWidth, cHeight, recInfo->Description(), false);
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
    
    ContentSet( recInfo->Description(), false, Theme.Color(clrMenuRecFontInfo), Theme.Color(clrMenuRecBg) );
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
    int Top = topBarHeight + marginItem + Config.decorBorderTopBarSize*2 + Config.decorBorderMenuContentHeadSize;
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
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
            Config.decorBorderMenuContentFg, Config.decorBorderMenuContentBg);
    else
        DecorBorderDraw(cLeft, cTop, cWidth, ContentGetTextHeight(), Config.decorBorderMenuContentSize, Config.decorBorderMenuContentType,
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