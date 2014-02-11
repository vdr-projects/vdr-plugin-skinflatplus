#pragma once

#define X_DISPLAY_MISSING


#include <vdr/osd.h>
#include <vdr/skins.h>

#include <Magick++.h>
#include "imagemagickwrapper.h"

#include "flat.h"

using namespace Magick;

class cImageLoader : public cImageMagickWrapper {
public:
    cImageLoader();
    ~cImageLoader();
    
    cImage* LoadLogo(const char *logo, int width, int height);
    cImage* LoadIcon(const char *cIcon, int width, int height, bool preserveAspect = true);
    
    //cImage GetImage();
    //cImage GetImage(int width, int height, bool preserveAspect = true);

    //bool LoadIcon(const char *cIcon, int size = -1);
    //bool LoadIcon2(const char *cIcon);
   
private:
    int epgImageWidthLarge, epgImageHeightLarge;
    int epgImageWidth, epgImageHeight;
    cString logoExtension;

    void toLowerCase(std::string &str);
};
