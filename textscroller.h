#pragma once

#include <list>
#include "flat.h"
#include <string.h>
#include <vdr/thread.h>

#define WAITSTEPS 50

class cTextScroll
{
private:
    cRect Position;

    tColor ColorFg, ColorBg;
    std::string Text;
    cFont *Font;
    cPixmap *Pixmap;
    cOsd *Osd;

    int waitSteps;
    bool isReserveStep;
    bool ResetX;
    int ScrollType;

public:
    cTextScroll(cOsd *osd, int type) {
        Font = NULL;
        Pixmap = NULL;
        Osd = osd;

        ScrollType = type;
        isReserveStep = false;
        waitSteps = WAITSTEPS;
        ResetX = false;
    }

    ~cTextScroll() {
        if( Pixmap ) {
            Osd->DestroyPixmap(Pixmap);
            Pixmap = NULL;
        }
    }

/*
    cTextScroll& operator=(const cTextScroll& other) {
        if( this != &other ) {
            this->Position = other.Position;
            this->Text = other.Text;
            this->ColorFg = other.ColorFg;
            this->ColorBg = other.ColorBg;
            this->Font = other.Font;
            this->Pixmap = other.Pixmap;
            this->Osd = other.Osd;
            dsyslog("operator= pointer: %x", this->Pixmap);
        }
        return *this;
    }
*/
    void Reset(void);
    void SetText(const char *text, cRect position, tColor colorFg, tColor colorBg, cFont *font);
    void DoStep(int Step);
    void Draw(void);

};

class cTextScrollers : public cThread
{
private:
    std::vector<cTextScroll *> Scrollers;

    cOsd *Osd;
    int scrollStep, scrollDelay;
    int scrollType;

    virtual void Action(void);
public:
    cTextScrollers();
    ~cTextScrollers();

    void Clear(void);
    void SetOsd(cOsd *osd) { Osd = osd;}
    void SetScrollStep(int step) { scrollStep = step; }
    void SetScrollDelay(int delay) { scrollDelay = delay; }
    void SetScrollType(int type) { scrollType = type; }
    void AddScroller(const char *text, cRect position, tColor colorFg, tColor colorBg, cFont *font);
};
