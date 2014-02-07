#include "displayreplay.h"

cFlatDisplayReplay::cFlatDisplayReplay(bool ModeOnly) {
    labelHeight = fontHeight + fontSmlHeight;
    current = "";
    total = "";

    ProgressShown = false;
    CreateFullOsd();
    TopBarCreate();
    MessageCreate();

    screenWidth = lastScreenWidth = -1;

    labelPixmap = osd->CreatePixmap(1, cRect(Config.decorBorderReplaySize, osdHeight - labelHeight - Config.decorBorderReplaySize,
        osdWidth - Config.decorBorderReplaySize*2, labelHeight));
    iconsPixmap = osd->CreatePixmap(2, cRect(Config.decorBorderReplaySize, osdHeight - labelHeight - Config.decorBorderReplaySize,
        osdWidth - Config.decorBorderReplaySize*2, labelHeight));
    
    ProgressBarCreate(Config.decorBorderReplaySize, osdHeight - labelHeight - Config.decorProgressReplaySize - Config.decorBorderReplaySize - marginItem,
        osdWidth - Config.decorBorderReplaySize*2, Config.decorProgressReplaySize, marginItem, 0,
        Config.decorProgressReplayFg, Config.decorProgressReplayBarFg, Config.decorProgressReplayBg, Config.decorProgressReplayType);

    labelJump = osd->CreatePixmap(1, cRect(Config.decorBorderReplaySize, 
        osdHeight - labelHeight - Config.decorProgressReplaySize - marginItem*3 - fontHeight - Config.decorBorderReplaySize*2,
        osdWidth - Config.decorBorderReplaySize*2, fontHeight));
    
    labelPixmap->Fill(Theme.Color(clrReplayBg));
    labelJump->Fill(clrTransparent);

}

cFlatDisplayReplay::~cFlatDisplayReplay() {
    osd->DestroyPixmap(labelPixmap);
    osd->DestroyPixmap(labelJump);
    osd->DestroyPixmap(iconsPixmap);
}

void cFlatDisplayReplay::SetRecording(const cRecording *Recording) {
    const cRecordingInfo *recInfo = Recording->Info();
    SetTitle( recInfo->Title() );
    cString info = "";
    if (recInfo->ShortText())
        info = cString::sprintf("%s - %s %s", recInfo->ShortText(), *ShortDateString(Recording->Start()), *TimeString(Recording->Start()));
    else
        info = cString::sprintf("%s %s", *ShortDateString(Recording->Start()), *TimeString(Recording->Start()));

    labelPixmap->DrawText(cPoint(marginItem, fontHeight), info, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), fontSml, osdWidth - Config.decorBorderReplaySize*2);
}

void cFlatDisplayReplay::SetTitle(const char *Title) {
    TopBarSetTitle(Title);
    TopBarSetMenuIcon("extraIcons/Playing");
    
}

void cFlatDisplayReplay::SetMode(bool Play, bool Forward, int Speed) {
    if( Setup.ShowReplayMode ) {
        int left = osdWidth - Config.decorBorderReplaySize*2 - (fontHeight * 4 + marginItem * 3);
        left /= 2;

        iconsPixmap->Fill(clrTransparent);
        labelPixmap->DrawRectangle(cRect( left - font->Width("33") - marginItem, 0, fontHeight*4 + marginItem*6 + font->Width("33")*2, fontHeight), Theme.Color(clrReplayBg) );

        cString rewind, pause, play, forward;
        cString speed;
        
        if( Speed == -1 ) {
            if( Play ) {
                rewind = "rewind";
                pause = "pause";
                play = "play_sel";
                forward = "forward";
            } else {
                rewind = "rewind";
                pause = "pause_sel";
                play = "play";
                forward = "forward";
            }                
        } else {
            speed = cString::sprintf("%d", Speed);
            if( Forward ) {
                rewind = "rewind";
                pause = "pause";
                play = "play";
                forward = "forward_sel";
                labelPixmap->DrawText(cPoint(left + fontHeight*4 + marginItem*4, 0), speed, Theme.Color(clrReplayFontSpeed), Theme.Color(clrReplayBg), font);
            } else {
                rewind = "rewind_sel";
                pause = "pause";
                play = "play";
                forward = "forward";
                labelPixmap->DrawText(cPoint(left - font->Width(speed) - marginItem, 0), speed, Theme.Color(clrReplayFontSpeed), Theme.Color(clrReplayBg), font);
            }                
        }

        if( imgLoader.LoadIcon(*rewind, fontHeight, fontHeight) )
            iconsPixmap->DrawImage( cPoint(left, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*pause, fontHeight, fontHeight) )
            iconsPixmap->DrawImage( cPoint(left + fontHeight + marginItem, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*play, fontHeight, fontHeight) )
            iconsPixmap->DrawImage( cPoint(left + fontHeight*2 + marginItem*2, 0), imgLoader.GetImage() );
        if( imgLoader.LoadIcon(*forward, fontHeight, fontHeight) )
            iconsPixmap->DrawImage( cPoint(left + fontHeight*3 + marginItem*3, 0), imgLoader.GetImage() );

    }
    
    if( ProgressShown )
        DecorBorderDraw(Config.decorBorderReplaySize, osdHeight - labelHeight - Config.decorProgressReplaySize - Config.decorBorderReplaySize - marginItem,
            osdWidth - Config.decorBorderReplaySize*2, labelHeight + Config.decorProgressReplaySize + marginItem,
            Config.decorBorderReplaySize, Config.decorBorderReplayType, Config.decorBorderReplayFg, Config.decorBorderReplayBg);
    else
        DecorBorderDraw(Config.decorBorderReplaySize, osdHeight - labelHeight - Config.decorBorderReplaySize,
            osdWidth - Config.decorBorderReplaySize*2, labelHeight,
            Config.decorBorderReplaySize, Config.decorBorderReplayType, Config.decorBorderReplayFg, Config.decorBorderReplayBg);
        
    ResolutionAspectDraw();
}

void cFlatDisplayReplay::SetProgress(int Current, int Total) {
    ProgressShown = true;
    ProgressBarDrawMarks(Current, Total, marks, Theme.Color(clrReplayMarkFg), Theme.Color(clrReplayMarkCurrentFg));
}

void cFlatDisplayReplay::SetCurrent(const char *Current) {
    current = Current;
    UpdateInfo();
}

void cFlatDisplayReplay::SetTotal(const char *Total) {
    total = Total;
    UpdateInfo();
}

void cFlatDisplayReplay::UpdateInfo(void) {
    int right = osdWidth - Config.decorBorderReplaySize*2 - font->Width(total);
    labelPixmap->DrawText(cPoint(marginItem, 0), current, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, font->Width(current), fontHeight);
    labelPixmap->DrawText(cPoint(right - marginItem, 0), total, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, font->Width(total), fontHeight);
}

void cFlatDisplayReplay::SetJump(const char *Jump) {
    DecorBorderClearByFrom(BorderRecordJump);
    
    if( !Jump )
    {
        labelJump->Fill(clrTransparent);
        return;
    }        
    int left = osdWidth - Config.decorBorderReplaySize*2 - font->Width(Jump);
    left /= 2;
    
    labelJump->DrawText(cPoint(left, 0), Jump, Theme.Color(clrReplayFont), Theme.Color(clrReplayBg), font, font->Width(Jump), fontHeight, taCenter);

    DecorBorderDraw(left + Config.decorBorderReplaySize,
    osdHeight - labelHeight - Config.decorProgressReplaySize - marginItem*3 - fontHeight - Config.decorBorderReplaySize*2,
        font->Width(Jump), fontHeight,
        Config.decorBorderReplaySize, Config.decorBorderReplayType, Config.decorBorderReplayFg, Config.decorBorderReplayBg, BorderRecordJump);
}

void cFlatDisplayReplay::ResolutionAspectDraw(void) {
    int left = osdWidth - Config.decorBorderReplaySize*2;
    int imageTop = 0;
    if( screenWidth > 0 ) {
    if( Config.RecordingResolutionAspectShow ) {         // Show Aspect
        cString asp = "unknown_asp";                     // ???
        if(Config.RecordingSimpleAspectFormat && screenWidth > 720) {
            asp = "hd";                                   // No aspect for HD
        } else {
            if( screenAspect == 4.0/3.0 )
                asp = "43";
            else if( screenAspect == 16.0/9.0 )
                asp = "169";
            else if( screenAspect == 2.21 )
                asp = "221";
        }
        if (imgLoader.LoadIcon(*asp, 999, fontSmlHeight)) {
            imageTop = fontHeight + (fontSmlHeight - imgLoader.Height())/2;
            left -= imgLoader.Width();
            iconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
            left -= marginItem*2;
        }
    }

    if( Config.RecordingResolutionAspectShow ) {         // Show Resolution
        cString res = "";
        switch (screenWidth) {
            case 1920:                        // 1920x1080 (HD1080 Full HDTV)
                res = "1920x1080";
                break;
            case 1440:                        // 1440x1080 (HD1080 DV)
                res = "1440x1080";
                break;
            case 1280:                        // 1280x720 (HD720)
                res = "1280x720";
                break;
            case 960:                         // 960x720 (HD720 DV)
                res = "960x720";
                break;
            case 720:                         // 720x576 (PAL)
                res = "720x576";
                break;
            case 544:                         // 544x576 (PAL)
                res = "544x576";
                break;
            case 528:                         // 528x576 (PAL)
                res = "528x576";
                break;
            case 480:                         // 480x576 (PAL SVCD)
                res = "480x576";
                break;
            case 352:                         // 352x576 (PAL CVD)
                res = "352x576";
                break;
            default:
                res = "unknown_res";         // TODO: Log resolution
                dsyslog("unkown resolution Width: %d Height: %d Aspect: %.2f\n", screenWidth, screenHeight, screenAspect);
                break;
            }

            //printf("Width: %d Height: %d Aspect: %.2f\n", screenWidth, screenHeight, screenAspect);
        
            if (imgLoader.LoadIcon(*res, 999, fontSmlHeight)) {
                imageTop = fontHeight + (fontSmlHeight - imgLoader.Height())/2;
                left -= imgLoader.Width();
                iconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
                left -= marginItem*2;
            }
        }
        if( Config.RecordingFormatShow && !Config.RecordingSimpleAspectFormat) {
            cString iconName("");        // Show Format
            switch (screenWidth) {
                case 1920:
                case 1440:
                case 1280:
                    iconName = "hd";
                    break;
                case 720:
                    iconName = "sd";
                    break;
                default:
                    iconName = "sd";
                    break;
            }

            if (imgLoader.LoadIcon(*iconName, 999, fontSmlHeight)) {
                imageTop = fontHeight + (fontSmlHeight - imgLoader.Height())/2;
                left -= imgLoader.Width();
                iconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
                left -= marginItem*2 ;
            }
        }
    }
}

void cFlatDisplayReplay::SetMessage(eMessageType Type, const char *Text) {
    if (Text)
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cFlatDisplayReplay::Flush(void) {
    TopBarUpdate();

    if( Config.RecordingResolutionAspectShow ) {
        cDevice::PrimaryDevice()->GetVideoSize(screenWidth, screenHeight, screenAspect);
        if (screenWidth != lastScreenWidth) {
            lastScreenWidth = screenWidth;
            ResolutionAspectDraw();
        }
    }

    osd->Flush();
}
