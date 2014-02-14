#pragma once

#include "baserender.h"
#include <list>

class cFlatDisplayMenu : public cFlatBaseRender,  public cSkinDisplayMenu {
    private:
        cPixmap *menuPixmap;
        cPixmap *menuIconsPixmap;
        int menuTop, menuWidth;
        int menuItemWidth;
    
        eMenuCategory menuCategory;
        int VideoDiskUsageState;
    
        int chLeft, chTop, chWidth, chHeight;
        cPixmap *contentHeadPixmap;
    
        int cLeft, cTop, cWidth, cHeight;
    
        cPixmap *scrollbarPixmap;
        int scrollBarTop, scrollBarWidth, scrollBarHeight;

        int itemHeight, itemChannelHeight, itemTimerHeight, itemEventHeight;

        std::list<sDecorBorder> ItemsBorder;
        sDecorBorder EventBorder, RecordingBorder, TextBorder;
    
        bool isScrolling;
        bool ShowEvent, ShowRecording, ShowText;
    
        void ItemBorderInsertUnique(sDecorBorder ib);
        void ItemBorderDrawAllWithScrollbar(void);
        void ItemBorderDrawAllWithoutScrollbar(void);
        void ItemBorderClear(void);

        static std::string items[16];
        std::string MainMenuText(std::string Text);
        cString GetIconName(std::string element);

        void DrawScrollbar(int Total, int Offset, int Shown, int Top, int Height, bool CanScrollUp, bool CanScrollDown, bool isContent = false);
        int ItemsHeight(void);
        bool CheckProgressBar(const char *text);
        void DrawProgressBarFromText(cRect rec, cRect recBg, const char *bar, tColor ColorFg, tColor ColorBarFg, tColor ColorBg);

        cBitmap *bmNew, *bmRec, *bmArrowTurn, *bmClock, *bmClocksml, *bmVPS;
        static cBitmap bmCNew, bmCRec, bmCArrowTurn, bmCClock, bmCClocksml, bmCHD, bmCVPS;
        void DrawItemExtraEvent(const cEvent *Event, cString EmptyText);
    public:
        cFlatDisplayMenu(void);
        virtual ~cFlatDisplayMenu();
        virtual void Scroll(bool Up, bool Page);
        virtual int MaxItems(void);
        virtual void Clear(void);
        
        virtual void SetMenuCategory(eMenuCategory MenuCategory);
        //virtual void SetTabs(int Tab1, int Tab2 = 0, int Tab3 = 0, int Tab4 = 0, int Tab5 = 0);
    
        virtual void SetTitle(const char *Title);
        virtual void SetButtons(const char *Red, const char *Green = NULL, const char *Yellow = NULL, const char *Blue = NULL);
        virtual void SetMessage(eMessageType Type, const char *Text);
        virtual void SetItem(const char *Text, int Index, bool Current, bool Selectable);
        
        virtual bool SetItemEvent(const cEvent *Event, int Index, bool Current, bool Selectable, const cChannel *Channel, bool WithDate, eTimerMatch TimerMatch);
        virtual bool SetItemTimer(const cTimer *Timer, int Index, bool Current, bool Selectable);
        virtual bool SetItemChannel(const cChannel *Channel, int Index, bool Current, bool Selectable, bool WithProvider);
        //virtual bool SetItemRecording(const cRecording *Recording, int Index, bool Current, bool Selectable, int Level, int Total, int New);

        virtual void SetScrollbar(int Total, int Offset);
        virtual void SetEvent(const cEvent *Event);
        virtual void SetRecording(const cRecording *Recording);
        virtual void SetText(const char *Text, bool FixedFont);
        virtual int GetTextAreaWidth(void) const;
        virtual const cFont *GetTextAreaFont(bool FixedFont) const;
        virtual void Flush(void);
};
