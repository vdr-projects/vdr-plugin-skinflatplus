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
    int Layer;

    int waitSteps;
    bool isReserveStep;
    bool ResetX;
    int ScrollType;
public:
    cTextScroll(cOsd *osd, int type, int layer) {
        Font = NULL;
        Pixmap = NULL;
        Osd = osd;
        Layer = layer;

        ScrollType = type;
        isReserveStep = false;
        waitSteps = WAITSTEPS;
        ResetX = false;
    }
    cTextScroll(cOsd *osd, int type) {
        Font = NULL;
        Pixmap = NULL;
        Osd = osd;
        Layer = 2;

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
    int Layer;
    virtual void Action(void);
public:
    cTextScrollers();
    ~cTextScrollers();

    void Clear(void);
    void SetOsd(cOsd *osd) { Osd = osd;}
    void SetPixmapLayer(int layer) { Layer = layer; }
    void SetScrollStep(int step) { scrollStep = step; }
    void SetScrollDelay(int delay) { scrollDelay = delay; }
    void SetScrollType(int type) { scrollType = type; }
    void AddScroller(const char *text, cRect position, tColor colorFg, tColor colorBg, cFont *font);
};
