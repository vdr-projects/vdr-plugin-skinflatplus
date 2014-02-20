#include "displaychannel.h"

cFlatDisplayChannel::cFlatDisplayChannel(bool WithInfo) {
    if (firstDisplay) {
        firstDisplay = false;
        doOutput = false;
        return;
    } else
        doOutput = true;  

    present = NULL;
    channelName = "";
    chanInfoTopPixmap = NULL;
    chanInfoBottomPixmap = NULL;
    chanLogoPixmap = NULL;
    chanIconsPixmap = NULL;

    isGroup = false;
    isRecording = false,
    isRadioChannel = false;

    screenWidth = lastScreenWidth = -1;
    
    CreateFullOsd();
    if ( !osd )
        return;
    TopBarCreate();
    MessageCreate();

    channelWidth = osdWidth - Config.decorBorderChannelSize*2;
    channelHeight = osdHeight - Config.decorBorderChannelSize*2;
    // von unten noch oben
    // 2*EPG + 2*EPGsml
    heightBottom = (fontHeight*2) + (fontSmlHeight*2) + marginItem; // Top, Buttom, Between
    heightImageLogo = heightBottom;
    if( Config.SignalQualityShow )
        heightBottom += max(fontSmlHeight, Config.decorProgressSignalSize) + marginItem;
    else if( Config.ChannelIconsShow )
        heightBottom += fontSmlHeight + marginItem;
        
    int heightTop = fontHeight;

    int height = heightBottom;
    chanInfoBottomPixmap = osd->CreatePixmap(1, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightBottom));
    chanInfoBottomPixmap->Fill( Theme.Color(clrChannelBg) );

    chanIconsPixmap = osd->CreatePixmap(2, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightBottom));
    chanIconsPixmap->Fill( clrTransparent );

    chanLogoPixmap = osd->CreatePixmap(2, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, heightBottom, heightBottom));
    chanLogoPixmap->Fill( clrTransparent );
    
    height += Config.decorProgressChannelSize + marginItem*2;
    ProgressBarCreate(Config.decorBorderChannelSize, Config.decorBorderChannelSize+channelHeight - height + marginItem,
        channelWidth, Config.decorProgressChannelSize, marginItem, 0,
        Config.decorProgressChannelFg, Config.decorProgressChannelBarFg, Config.decorProgressChannelBg, Config.decorProgressChannelType);

    ProgressBarDrawBgColor();
    
    height += heightTop;
    chanInfoTopPixmap = osd->CreatePixmap(1, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightTop));
    chanInfoTopPixmap->Fill( clrTransparent );
    
    DecorBorderDraw(Config.decorBorderChannelSize, Config.decorBorderChannelSize+channelHeight - height,
        channelWidth, heightTop + heightBottom + Config.decorProgressChannelSize+marginItem*2,
        Config.decorBorderChannelSize, Config.decorBorderChannelType, Config.decorBorderChannelFg, Config.decorBorderChannelBg);
}

cFlatDisplayChannel::~cFlatDisplayChannel() {
    if( !doOutput )
        return;
    if (osd) {
        if( chanInfoTopPixmap )
            osd->DestroyPixmap(chanInfoTopPixmap);
        if( chanInfoBottomPixmap )
            osd->DestroyPixmap(chanInfoBottomPixmap);
        if( chanLogoPixmap )
            osd->DestroyPixmap(chanLogoPixmap);
        if( chanIconsPixmap )
            osd->DestroyPixmap(chanIconsPixmap);
    }
}

void cFlatDisplayChannel::SetChannel(const cChannel *Channel, int Number) {
    if( !doOutput )
        return;
    cString channelNumber("");
    isRecording = false;
    chanIconsPixmap->Fill( clrTransparent );
    lastScreenWidth = -1;
    
    if (Channel) {
        isRadioChannel = ((!Channel->Vpid())&&(Channel->Apid(0))) ? true : false;
        isGroup = Channel->GroupSep();
        
        channelName = Channel->Name();
        if (!Channel->GroupSep())
            channelNumber = cString::sprintf("%d%s", Channel->Number(), Number ? "-" : "");
        else if (Number)
            channelNumber = cString::sprintf("%d-", Number);
        
        CurChannel = Channel;
    } else
        channelName = ChannelString(NULL, 0);

    cString channelString = cString::sprintf("%s  %s", *channelNumber, *channelName);

    chanInfoTopPixmap->Fill(Theme.Color(clrChannelBg));
    chanInfoTopPixmap->DrawText(cPoint(50, 0), channelString, Theme.Color(clrChannelFontTitle), Theme.Color(clrChannelBg), font);

    chanLogoPixmap->Fill(clrTransparent);
    int imageHeight = heightImageLogo - marginItem*2;
    int imageLeft = marginItem*2;
    int imageTop = marginItem;
    cImage *img = imgLoader.LoadLogo(*channelName, imageHeight, imageHeight);
    if( img ) {
        chanLogoPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
    } else if( !isGroup ) { // draw default logo
        if( isRadioChannel ) {
            img = imgLoader.LoadIcon("radio", imageHeight, imageHeight);
            if( img ) {
                chanLogoPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
            }
        } else {
            img = imgLoader.LoadIcon("tv", imageHeight, imageHeight);
            if( img ) {
                chanLogoPixmap->DrawImage( cPoint(imageLeft, imageTop), *img );
            }
        }
    }
}

void cFlatDisplayChannel::ChannelIconsDraw(const cChannel *Channel, bool Resolution) {
    if( !Resolution ) {
        chanIconsPixmap->Fill( clrTransparent );
    }

    int width = fontSmlHeight;
    int top = fontHeight*2 + fontSmlHeight*2 + marginItem;
    int height = fontSmlHeight;
    int imageTop = 0;
    cImage *img = NULL;
    
    if( Config.SignalQualityShow ) {
        top += max(fontSmlHeight, Config.decorProgressSignalSize) / 2 - fontSmlHeight / 2;
        height = max(fontSmlHeight, Config.decorProgressSignalSize);
    } else
        top += fontSmlHeight / 2 - fontSmlHeight / 2;

    int left = channelWidth - width - marginItem*2;

    if( Channel ) {
        if (Channel->Ca()) {
            img = imgLoader.LoadIcon("crypted", 999, height);
            if( img ) {
                imageTop = top + (height - img->Height())/2;
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), *img);
                left -= marginItem*2;
            }
        } else {
             img = imgLoader.LoadIcon("uncrypted", 999, height);
            if( img ) {
                imageTop = top + (height - img->Height())/2;
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), *img);
                left -= marginItem*2;
            }
        }
    }

    if( Resolution && !isRadioChannel && screenWidth > 0 ) {
        if( Config.ChannelResolutionAspectShow ) {     // Show Aspect
            cString asp = "unknown_asp";                 // ???
            if(Config.ChannelSimpleAspectFormat && screenWidth > 720) {
                asp = "hd";                                // No aspect for HD
            } else {
                if( screenAspect == 4.0/3.0 )
                    asp = "43";
                else if( screenAspect == 16.0/9.0 )
                    asp = "169";
                else if( screenAspect == 2.21 )
                    asp = "221";
            }
            img = imgLoader.LoadIcon(*asp, 999, height);
            if( img ) {
                imageTop = top + (height - img->Height())/2;
                left -= img->Width();
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), *img);
                left -= marginItem*2;
            }
        }

        if( Config.ChannelResolutionAspectShow ) {    // Show Resolution
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
            case 704:                         // 704x576 (PAL)
                res = "704x576";
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
                res = "unknown_res";         // TODO: Add Log line of resolution
                dsyslog("unkown resolution Width: %d Height: %d Aspect: %.2f\n", screenWidth, screenHeight, screenAspect);
                break;
            }

            img = imgLoader.LoadIcon(*res, 999, height);
            if( img ) {
                imageTop = top + (height - img->Height())/2;
                left -= img->Width();
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), *img);
                left -= marginItem*2;
            }
        }
        if( Config.ChannelFormatShow && !Config.ChannelSimpleAspectFormat) {
            cString iconName("");      // Show Format
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

            img = imgLoader.LoadIcon(*iconName, 999, height);
            if( img ) {
                imageTop = top + (height - img->Height())/2;
                left -= img->Width();
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), *img);
                left -= marginItem*2;
            }
        }
    }
}

void cFlatDisplayChannel::SetEvents(const cEvent *Present, const cEvent *Following) {
    if( !doOutput )
        return;

    present = Present;
    cString epgShort("");
    cString epg("");

    chanInfoBottomPixmap->Fill(Theme.Color(clrChannelBg));
    chanIconsPixmap->Fill( clrTransparent );

    bool isRec = false;
    int RecWidth = fontSml->Width("REC");
    
    int left = heightBottom + marginItem;

    if( Present ) {
        cString startTime = Present->GetTimeString();
        cString endTime = Present->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = font->Width(Present->Title());
        int epgShortWidth = fontSml->Width(Present->ShortText());

        if( Present->HasTimer() ) {
            isRec = true;
            epgWidth += marginItem + RecWidth;
        }
        
        int s = (int)(time(NULL) - Present->StartTime()) / 60;
        int sleft = (Present->Duration() / 60) - s;

        cString seen = cString::sprintf("%d-/%d+ %d min", s, sleft, Present->Duration() / 60);
        int seenWidth = fontSml->Width(*seen);

        if( epgWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Present->Title(), font, channelWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
            epgWidth = font->Width(*epg);
            if( Present->HasTimer() ) {
                isRec = true;
                epgWidth += marginItem + RecWidth;
            }
        } else {
            epg = Present->Title();
        }

        if( epgShortWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = fontSml->Width("... ");
            cTextWrapper epgInfoWrapper(Present->ShortText(), fontSml, channelWidth - left - timeStringWidth - dotsWidth);
            epgShort = epgInfoWrapper.GetLine(0);
            epgShort = cString::sprintf("%s...", *epgShort);
        } else {
            epgShort = Present->ShortText();
        }
        
        chanInfoBottomPixmap->DrawText(cPoint(channelWidth - timeStringWidth - marginItem * 2, 0), *timeString,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
        chanInfoBottomPixmap->DrawText(cPoint(channelWidth - seenWidth - marginItem * 2, fontSmlHeight), *seen,
                Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);
        
        chanInfoBottomPixmap->DrawText(cPoint(left, 0), *epg, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), font);
        chanInfoBottomPixmap->DrawText(cPoint(left, fontHeight), *epgShort, Theme.Color(clrChannelFontEpg), Theme.Color(clrChannelBg), fontSml);

        if( isRec ) {
            chanInfoBottomPixmap->DrawText(cPoint(left + epgWidth + marginItem - RecWidth, 0), "REC",
                Theme.Color(clrChannelRecordingPresentFg), Theme.Color(clrChannelRecordingPresentBg), fontSml);
        }
    }
    
    if( Following ) {
        isRec = false;
        cString startTime = Following->GetTimeString();
        cString endTime = Following->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = font->Width(Following->Title());
        int epgShortWidth = fontSml->Width(Following->ShortText());

        if( Following->HasTimer() ) {
            epgWidth += marginItem + RecWidth;
            isRec = true;
        }
        
        cString dur = cString::sprintf("%d min", Following->Duration() / 60);
        int durWidth = fontSml->Width(*dur);

        if( epgWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Following->Title(), font, channelWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
            epgWidth = font->Width(*epg);
            if( Following->HasTimer() ) {
                isRec = true;
                epgWidth += marginItem + RecWidth;
            }
        } else {
            epg = Following->Title();
        }
        if (epgShortWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = fontSml->Width("... ");
            cTextWrapper epgInfoWrapper(Following->ShortText(), fontSml, channelWidth - left - timeStringWidth - dotsWidth);
            epgShort = epgInfoWrapper.GetLine(0);
            epgShort = cString::sprintf("%s...", *epgShort);
        } else {
            epgShort = Following->ShortText();
        }

        chanInfoBottomPixmap->DrawText(cPoint(channelWidth - timeStringWidth - marginItem * 2, fontHeight + fontSmlHeight), *timeString,
                Theme.Color(clrChannelFontEpgFollow), Theme.Color(clrChannelBg), fontSml);
        chanInfoBottomPixmap->DrawText(cPoint(channelWidth - durWidth - marginItem * 2, fontHeight + fontSmlHeight*2), *dur,
                Theme.Color(clrChannelFontEpgFollow), Theme.Color(clrChannelBg), fontSml);

        chanInfoBottomPixmap->DrawText(cPoint(left, fontHeight + fontSmlHeight), *epg,
                Theme.Color(clrChannelFontEpgFollow), Theme.Color(clrChannelBg), font);
        chanInfoBottomPixmap->DrawText(cPoint(left, fontHeight*2 + fontSmlHeight), *epgShort,
                Theme.Color(clrChannelFontEpgFollow), Theme.Color(clrChannelBg), fontSml);

        if( isRec ) {
            chanInfoBottomPixmap->DrawText(cPoint(left + epgWidth + marginItem - RecWidth, fontHeight + fontSmlHeight), "REC",
                Theme.Color(clrChannelRecordingFollowFg), Theme.Color(clrChannelRecordingFollowBg), fontSml);
        }
    }
    if( Config.ChannelIconsShow && CurChannel ) {
        ChannelIconsDraw(CurChannel, false);
    }
}

void cFlatDisplayChannel::SetMessage(eMessageType Type, const char *Text) {
    if( !doOutput )
        return;

    // Wenn es einen Text gibt, diesen Anzeigen ansonsten Message ausblenden
    if( Text )
        MessageSet(Type, Text);
    else
        MessageClear();
}

void cFlatDisplayChannel::SignalQualityDraw(void) {
    int SignalStrength = cDevice::ActualDevice()->SignalStrength();
    int SignalQuality = cDevice::ActualDevice()->SignalQuality();
    int signalWidth = channelWidth / 2;
    
    int top = fontHeight*2 + fontSmlHeight*2 + marginItem;
    top += max(fontSmlHeight, Config.decorProgressSignalSize) / 2 - fontSmlHeight / 2;
    int left = marginItem;
    int progressTop = fontHeight*2 + fontSmlHeight*2 + marginItem;
    progressTop += max(fontSmlHeight, Config.decorProgressSignalSize) / 2 - Config.decorProgressSignalSize / 2;
    
    chanInfoBottomPixmap->DrawText(cPoint(left, top), "STR",
        Theme.Color(clrChannelSignalFont), Theme.Color(clrChannelBg), fontSml);
    int progressLeft = left + fontSml->Width("STR") + marginItem;
    int progressWidth = signalWidth / 2 - progressLeft - marginItem;
    ProgressBarDrawRaw(chanInfoBottomPixmap, chanInfoBottomPixmap, cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize),
        cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize), SignalStrength, 100,
        Config.decorProgressSignalFg, Config.decorProgressSignalBarFg, Config.decorProgressSignalBg, Config.decorProgressSignalType, false);

    left = signalWidth / 2 + marginItem;
    chanInfoBottomPixmap->DrawText(cPoint(left, top), "SNR",
        Theme.Color(clrChannelSignalFont), Theme.Color(clrChannelBg), fontSml);
    progressLeft = signalWidth / 2 + marginItem + fontSml->Width("STR") + marginItem*2;
    progressWidth = signalWidth - progressLeft - marginItem;
    
    ProgressBarDrawRaw(chanInfoBottomPixmap, chanInfoBottomPixmap, cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize),
        cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize), SignalQuality, 100,
        Config.decorProgressSignalFg, Config.decorProgressSignalBarFg, Config.decorProgressSignalBg, Config.decorProgressSignalType, false);
}

void cFlatDisplayChannel::Flush(void) {
    if( !doOutput )
        return;

    int Current = 0;
    int Total = 0;
    if( present ) {
        time_t t = time(NULL);
        if (t > present->StartTime())
            Current = t - present->StartTime();
        Total = present->Duration();
        ProgressBarDraw(Current, Total);
    }
    if( Config.SignalQualityShow )
        SignalQualityDraw();
    
    if( Config.ChannelIconsShow ) {
        cDevice::PrimaryDevice()->GetVideoSize(screenWidth, screenHeight, screenAspect);
        if (screenWidth != lastScreenWidth) {
            lastScreenWidth = screenWidth;
            ChannelIconsDraw(CurChannel, true);
        }
    }
    TopBarUpdate();
    osd->Flush();
}
