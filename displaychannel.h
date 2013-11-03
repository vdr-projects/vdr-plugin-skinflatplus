#pragma once

#include "baserender.h"

class cFlatDisplayChannel : public cFlatBaseRender, public cSkinDisplayChannel {
    private:
        const cEvent *present;

        int channelWidth, channelHeight;
    
        cString channelName;
        const cChannel *CurChannel;
    
        cPixmap *chanInfoTopPixmap;
        cPixmap *chanInfoBottomPixmap;
        cPixmap *chanLogoPixmap;
        cPixmap *chanIconsPixmap;
    
        int screenWidth, lastScreenWidth;
        int heightBottom;
    
        bool isRecording;
        void SignalQualityDraw(void);
        void ChannelIconsDraw(const cChannel *Channel, bool Resolution);
    
    public:
        cFlatDisplayChannel(bool WithInfo);
        virtual ~cFlatDisplayChannel();
        virtual void SetChannel(const cChannel *Channel, int Number);
        virtual void SetEvents(const cEvent *Present, const cEvent *Following);
        virtual void SetMessage(eMessageType Type, const char *Text);
        virtual void Flush(void);
};
