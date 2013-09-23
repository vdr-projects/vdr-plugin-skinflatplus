#include "displaychannel.h"

cFlatDisplayChannel::cFlatDisplayChannel(bool WithInfo) {
    present = NULL;
    channelName = "";
    chanInfoTopPixmap = NULL;
    chanInfoBottomPixmap = NULL;
    chanLogoPixmap = NULL;
    chanIconsPixmap = NULL;

    screenWidth = lastScreenWidth = 0;
    
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
    if( Config.SignalQualityShow )
        heightBottom += max(fontSmlHeight, Config.decorProgressSignalSize) + marginItem;
    else if( Config.ChannelIconsShow )
        heightBottom += fontSmlHeight + marginItem;
        
    int heightTop = fontHeight;

    int height = heightBottom;
    chanInfoBottomPixmap = osd->CreatePixmap(1, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightBottom));

    chanIconsPixmap = osd->CreatePixmap(2, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightBottom));
    chanLogoPixmap = osd->CreatePixmap(2, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, heightBottom, heightBottom));
    
    height += Config.decorProgressChannelSize + marginItem*2;
    ProgressBarCreate(Config.decorBorderChannelSize, Config.decorBorderChannelSize+channelHeight - height + marginItem,
        channelWidth, Config.decorProgressChannelSize, marginItem, 0,
        Config.decorProgressChannelFg, Config.decorProgressChannelBarFg, Config.decorProgressChannelBg, Config.decorProgressChannelType);

    height += heightTop;
    chanInfoTopPixmap = osd->CreatePixmap(1, cRect(Config.decorBorderChannelSize,
        Config.decorBorderChannelSize+channelHeight - height, channelWidth, heightTop));

    DecorBorderDraw(Config.decorBorderChannelSize, Config.decorBorderChannelSize+channelHeight - height,
        channelWidth, heightTop + heightBottom + Config.decorProgressChannelSize+marginItem*2,
        Config.decorBorderChannelSize, Config.decorBorderChannelType, Config.decorBorderChannelFg, Config.decorBorderChannelBg);
}

cFlatDisplayChannel::~cFlatDisplayChannel() {
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
    cString channelNumber("");
    isRecording = false;
    if (Channel) {
        channelName = Channel->Name();
        if (!Channel->GroupSep())
            channelNumber = cString::sprintf("%d%s", Channel->Number(), Number ? "-" : "");
        else if (Number)
            channelNumber = cString::sprintf("%d-", Number);
        
        if( Config.ChannelIconsShow )
            ChannelIconsDraw(Channel, false);
    } else
        channelName = ChannelString(NULL, 0);

    cString channelString = cString::sprintf("%s  %s", *channelNumber, *channelName);

    chanInfoTopPixmap->Fill(Theme.Color(clrChannelBg));
    chanInfoTopPixmap->DrawText(cPoint(50, 0), channelString, Theme.Color(clrChannelFontTitle), Theme.Color(clrChannelBg), font);
}

void cFlatDisplayChannel::ChannelIconsDraw(const cChannel *Channel, bool Resolution) {
    if( !Resolution )
        chanIconsPixmap->Fill(clrTransparent);

    int width = fontSmlHeight;
    int top = fontHeight*2 + fontSmlHeight*2 + marginItem;
    int height = fontSmlHeight;
    
    if( Config.SignalQualityShow ) {
        top += max(fontSmlHeight, Config.decorProgressSignalSize) / 2 - fontSmlHeight / 2;
        height = max(fontSmlHeight, Config.decorProgressSignalSize);
    } else
        top += fontSmlHeight / 2 - fontSmlHeight / 2;

    int left = channelWidth - width - marginItem;

    // look for timers & records
    time_t t = time(NULL);
    for(cTimer *ti = Timers.First(); ti; ti = Timers.Next(ti)) {
        if(ti->Matches(t) && (ti->Channel() == Channel)) {
            if( ti->Recording() ) {
                isRecording = true;
                if( !Resolution ) {
                    if (imgLoader.LoadIcon("record", width)) {
                        int imageTop = top + (height - imgLoader.Height())/2;
                        chanIconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
                    }
                }
            }
        }
    }
    if( isRecording )
        left -= marginItem + width;
    
    if( !Resolution && Channel ) {
        if (Channel->Ca()) {
            if (imgLoader.LoadIcon("crypted", width)) {
                int imageTop = top + (height - imgLoader.Height())/2;
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
            }
        } else {
            if (imgLoader.LoadIcon("uncrypted", width)) {
                int imageTop = top + (height - imgLoader.Height())/2;
                chanIconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
            }
        }
    }
    left -= marginItem + width;

    if( Resolution ) {
        cString iconName("");
        switch (screenWidth) {
            case 1920:
            case 1440:
            case 1280:
                iconName = "hd3";
                break;
            case 720:
                iconName = "sd3";
                break;
            default:
                iconName = "sd3";
                break;
        }
        if (imgLoader.LoadIcon(*iconName, width)) {
            int imageTop = top + (height - imgLoader.Height())/2;
            //chanIconsPixmap->DrawRectangle(cRect(left, 0, imgLoader.Height(), height), Theme.Color(clrChannelBg));
            chanIconsPixmap->DrawImage(cPoint(left, imageTop), imgLoader.GetImage());
        }
    }
}

void cFlatDisplayChannel::SetEvents(const cEvent *Present, const cEvent *Following) {
    present = Present;
    cString epgShort("");
    cString epg("");

    chanInfoBottomPixmap->Fill(Theme.Color(clrChannelBg));
    chanLogoPixmap->Fill(clrTransparent);

    int imageHeight = heightBottom - marginItem*2;
    if( imgLoader.LoadLogo(*channelName, imageHeight, imageHeight) ) {
        int imageLeft = marginItem;
        int imageTop = (heightBottom - imgLoader.Height() ) / 2;
        
        chanLogoPixmap->DrawImage( cPoint(imageLeft, imageTop), imgLoader.GetImage() );
    }

    int left = heightBottom + marginItem;

    if( Present ) {
        cString startTime = Present->GetTimeString();
        cString endTime = Present->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = font->Width(Present->Title());
        int epgShortWidth = fontSml->Width(Present->ShortText());
        
        int s = (int)(time(NULL) - Present->StartTime()) / 60;
        int sleft = (Present->Duration() / 60) - s;

        cString seen = cString::sprintf("%d-/%d+ %d min", s, sleft, Present->Duration() / 60);
        int seenWidth = fontSml->Width(*seen);

        if( epgWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Present->Title(), font, channelWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
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

    }
    if( Following ) {
        cString startTime = Following->GetTimeString();
        cString endTime = Following->GetEndTimeString();

        cString timeString = cString::sprintf("%s - %s", *startTime, *endTime);
        int timeStringWidth = fontSml->Width(*timeString);

        int epgWidth = fontSml->Width(Following->Title());
        int epgShortWidth = fontSml->Width(Following->ShortText());
        
        cString dur = cString::sprintf("%d min", Following->Duration() / 60);
        int durWidth = fontSml->Width(*dur);

        if( epgWidth > channelWidth - left - timeStringWidth ) {
            int dotsWidth = font->Width("... ");
            cTextWrapper epgInfoWrapper(Following->Title(), font, channelWidth - left - timeStringWidth - dotsWidth);
            epg = epgInfoWrapper.GetLine(0);
            epg = cString::sprintf("%s...", *epg);
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
    }
}

void cFlatDisplayChannel::SetMessage(eMessageType Type, const char *Text) {
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
        Config.decorProgressSignalFg, Config.decorProgressSignalBarFg, Config.decorProgressSignalBg, Config.decorProgressSignalType);

    left = signalWidth / 2 + marginItem;
    chanInfoBottomPixmap->DrawText(cPoint(left, top), "SNR",
        Theme.Color(clrChannelSignalFont), Theme.Color(clrChannelBg), fontSml);
    progressLeft = signalWidth / 2 + marginItem + fontSml->Width("STR") + marginItem*2;
    progressWidth = signalWidth - progressLeft - marginItem;
    
    ProgressBarDrawRaw(chanInfoBottomPixmap, chanInfoBottomPixmap, cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize),
        cRect(progressLeft, progressTop, progressWidth, Config.decorProgressSignalSize), SignalQuality, 100,
        Config.decorProgressSignalFg, Config.decorProgressSignalBarFg, Config.decorProgressSignalBg, Config.decorProgressSignalType);
}

void cFlatDisplayChannel::Flush(void) {
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
        int screenHeight;
        double aspect;
        cDevice::PrimaryDevice()->GetVideoSize(screenWidth, screenHeight, aspect);
        if (screenWidth != lastScreenWidth) {
            ChannelIconsDraw(NULL, true);
            screenWidth = lastScreenWidth;
        }
    }
    TopBarUpdate();
    osd->Flush();
}
