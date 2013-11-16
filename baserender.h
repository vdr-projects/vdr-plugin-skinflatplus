#pragma once

#include "flat.h"
#include "imageloader.h"
#include <list>

enum eBorder {
    BorderMenuItem = 1,
    BorderRecordJump = 2,
    BorderMenuRecord = 3
};

struct sBorderFrom {
    int From;
    int Left, Top, Width, Height, Size;
};

class cFlatBaseRender
{
    protected:
        cOsd *osd;

        int osdLeft, osdTop, osdWidth, osdHeight;
        int marginItem;
    
        // Standard fonts
        cFont *font;
        cFont *fontSml;
        cFont *fontFixed;
        int fontHeight;
        int fontSmlHeight;
        int fontFixedHeight;

        // TopBar
        cPixmap *topBarPixmap;
        cPixmap *topBarExtraIconPixmap;
        cString topBarTitle;
        cString tobBarTitleExtra1, tobBarTitleExtra2;
        cString topBarExtraIcon;
        bool topBarExtraIconSet;
        
        bool topBarUpdateTitle;
        cString topBarLastDate;
        int topBarHeight;

        // Progressbar
        cPixmap *progressBarPixmap;
        cPixmap *progressBarPixmapBg;
        int progressBarHeight, progressBarTop, progressBarWidth, progressBarMarginHor, progressBarMarginVer;
        int ProgressType;
        tColor progressBarColorFg, progressBarColorBarFg, progressBarColorBarCurFg, progressBarColorBg;
        tColor progressBarColorMark, progressBarColorMarkCurrent;
        
        // Scrollbar
        int scrollBarWidth;
        
        // Buttons rot, grün, gelb, blau
        cPixmap *buttonsPixmap;
        int buttonsWidth, buttonsHeight, buttonsTop;
        int marginButtonColor, buttonColorHeight;

        // Nachricht
        cPixmap *messagePixmap;
        int messageWidth, messageHeight;

        // Mehrzeiliger Content mit Scrollbalken
        cPixmap *contentPixmap;
        int contentLeft, contentTop, contentHeight, contentWidth;
        int contentDrawPortHeight; // gesamthöhe des Textes
        bool contentHasScrollbar;
        bool contentShown;
        bool contentFixedFont;
        
        tColor contentColorFg, contentColorBg;
        cTextWrapper contentWrapper;

        cPixmap *decorPixmap;
        std::list<sBorderFrom> Borders; // for clear specific Borders (clear only MenuItems and not TopBar)

        void contentDraw(void);
        double ScrollbarSize(void);

        void ProgressBarDrawMark(int posMark, int posMarkLast, int posCurrent, bool Start, bool isCurrent);
        int ProgressBarMarkPos(int P, int Total);

        void DecorDrawGlowRectHor(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        void DecorDrawGlowRectVer(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        
        void DecorDrawGlowRectTL(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        void DecorDrawGlowRectTR(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        void DecorDrawGlowRectBL(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        void DecorDrawGlowRectBR(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg);
        
        void DecorDrawGlowEllipseTL(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg, int type);
        void DecorDrawGlowEllipseTR(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg, int type);
        void DecorDrawGlowEllipseBL(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg, int type);
        void DecorDrawGlowEllipseBR(cPixmap *pixmap, int Left, int Top, int Width, int Height, tColor ColorBg, int type);

        //tColor Multiply(tColor Color, uint8_t Alpha);
        tColor SetAlpha(tColor Color, double am);
    public:
        cImageLoader imgLoader;

        cFlatBaseRender(void);
        ~cFlatBaseRender(void);

        void CreateFullOsd(void);
        void CreateOsd(int Left, int Top, int Width, int Height);

        void TopBarCreate(void);
        void TopBarSetTitle(cString title);
        void TopBarSetTitleExtra(cString extra1, cString extra2);
        void TopBarSetExtraIcon(cString icon);
        void TopBarUpdate(void);

        void ButtonsCreate(void);
        void ButtonsSet(const char *Red, const char *Green = NULL, const char *Yellow = NULL, const char *Blue = NULL);

        void MessageCreate(void);
        void MessageSet(eMessageType Type, const char *Text);
        void MessageClear(void);

        void ProgressBarDrawRaw(cPixmap *Pixmap, cPixmap *PixmapBg, cRect rec, cRect recBg, int Current, int Total, tColor ColorFg, tColor ColorBarFg, tColor ColorBg, int Type);
        void ProgressBarCreate(int Left, int Top, int Width, int Height, int MarginHor, int MarginVer, tColor ColorFg, tColor ColorBarFg, tColor ColorBg, int Type);
        void ProgressBarDraw(int Current, int Total);
        void ProgressBarDrawMarks(int Current, int Total, const cMarks *Marks, tColor Color, tColor ColorCurrent);

        void ScrollbarDraw(cPixmap *Pixmap, int Left, int Top, int Height, int Total, int Offset, int Shown, bool CanScrollUp, bool CanScrollDown);
        int ScrollBarWidth(void);
        
        void ContentCreate(int Left, int Top, int Width, int Height, bool FixedFont);
        void ContentSet(const char *Text, bool FixedFont, tColor ColorFg, tColor ColorBg);
        bool ContentIsShown(void);
        bool ContentScrollable(void);
        bool ContentWillItBeScrollable(int Width, int Height, const char *Text, bool FixedFont);
        int ContentScrollTotal(void);
        int ContentScrollOffset(void);
        int ContentVisibleLines(void);
        int ContentGetHeight(void);
        bool ContentScroll(bool Up, bool Page);
        void ContentClear(void);
        
        void DecorBorderDraw(int Left, int Top, int Width, int Height, int Size, int Type, tColor ColorFg, tColor ColorBg, int From = 0);
        void DecorBorderClear(int Left, int Top, int Width, int Height, int Size);
        void DecorBorderClearAll(void);
        void DecorBorderClearByFrom(int From);
};
