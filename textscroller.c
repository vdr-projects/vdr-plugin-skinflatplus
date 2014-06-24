#include "textscroller.h"

void cTextScroll::SetText(const char *text, cRect position, tColor colorFg, tColor colorBg, cFont *font) {
    if( Osd == NULL )
        return;

    Text = text;

    Font = font;
    Position = position;

    ColorFg = colorFg; ColorBg = colorBg;
    cRect drawPort(0, 0, font->Width(Text.c_str()), Position.Height());

    if( Osd && Pixmap )
        Osd->DestroyPixmap(Pixmap);

    Pixmap = Osd->CreatePixmap(Layer, Position, drawPort);
    Pixmap->Fill( colorBg );
    Draw();
}

void cTextScroll::Reset(void) {
    if( !Pixmap )
        return;

    Pixmap->SetDrawPortPoint(cPoint(0, 0));
    waitSteps = WAITSTEPS;
}

void cTextScroll::Draw(void) {
    if( !Pixmap )
        return;
    Pixmap->DrawText(cPoint(0, 0), Text.c_str(), ColorFg, ColorBg, Font);
}

void cTextScroll::DoStep(int Step) {
    if( !Pixmap )
        return;

    // wait at the beginning for better read
    if( waitSteps > 0 ) {
        waitSteps--;
        return;
    }
    // wait after return to the front
    if( ResetX ) {
        ResetX = false;
        Pixmap->SetDrawPortPoint(cPoint(0, 0));
        waitSteps = WAITSTEPS;
        return;
    }

    int drawPortX = Pixmap->DrawPort().X();

    if( isReserveStep )
        drawPortX += Step;
    else
        drawPortX -= Step;

    int maxX = Pixmap->DrawPort().Width() - Pixmap->ViewPort().Width();
    maxX *= -1;

    if( ScrollType == 0 ) {
        if( drawPortX <= maxX ) {
            drawPortX += Step;
            ResetX = true;
            waitSteps = WAITSTEPS;
        }
    } else if( ScrollType == 1 ) {
        if( drawPortX <= maxX ) {
            isReserveStep = true;
            waitSteps = WAITSTEPS;
        } else if( drawPortX > 0 ) {
            isReserveStep = false;
            waitSteps = WAITSTEPS;
        }

    }

    Pixmap->SetDrawPortPoint(cPoint(drawPortX, 0));
}

cTextScrollers::cTextScrollers() {
    Layer = 2;
}

cTextScrollers::~cTextScrollers() {
}

void cTextScrollers::Clear(void) {
    Cancel(-1);
    while( Active() )
        cCondWait::SleepMs(10);

    std::vector<cTextScroll *>::iterator it;
    for( it = Scrollers.begin(); it != Scrollers.end(); it++) {
        delete *it;
    }

    Scrollers.clear();
}

void cTextScrollers::AddScroller(const char *text, cRect position, tColor colorFg, tColor colorBg, cFont *font) {
    Cancel(-1);
    while (Active())
        cCondWait::SleepMs(10);

    Scrollers.push_back( new cTextScroll(Osd, scrollType, Layer) );
    Scrollers.back()->SetText(text, position, colorFg, colorBg, font);

    if( !Running() ) {
        Start();
    }
}

void cTextScrollers::Action(void) {
    std::vector<cTextScroll *>::iterator it;
    for( it = Scrollers.begin(); it != Scrollers.end(); it++) {
        if( !Running() )
            return;
        cPixmap::Lock();
        (*it)->Reset();
        cPixmap::Unlock();
    }

    while( Running() ) {
        std::vector<cTextScroll *>::iterator it;
        for( it = Scrollers.begin(); it != Scrollers.end(); it++) {
            if( !Running() )
                return;
            cPixmap::Lock();
            (*it)->DoStep(scrollStep);
            cPixmap::Unlock();
        }

        if (Running())
            Osd->Flush();

        if (Running())
            cCondWait::SleepMs(scrollDelay);
    }
}

