#pragma once

#define X_DISPLAY_MISSING


#include <vdr/osd.h>
#include <vdr/skins.h>

#include <Magick++.h>
#include "imagemagickwrapper.h"

using namespace Magick;

class cImageLoader : public cImageMagickWrapper {
public:
    cImageLoader();
    ~cImageLoader();

    cImage* LoadLogo(const char *logo, int width, int height);
    cImage* LoadIcon(const char *cIcon, int width, int height, bool preserveAspect = true);
    cImage* LoadFile(const char *cFile, int width, int height, bool preserveAspect = true);
    bool FileExits(const std::string& name);
private:
    int epgImageWidthLarge, epgImageHeightLarge;
    int epgImageWidth, epgImageHeight;
    cString logoExtension;

    void toLowerCase(std::string &str);
};
