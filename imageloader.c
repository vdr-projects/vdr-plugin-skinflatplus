#include "imageloader.h"
#include <math.h>
#include <string>
#include <dirent.h>
#include <iostream>

using namespace Magick;

cImageLoader::cImageLoader() {
    InitializeMagick(NULL);
    logoExtension = "png";
}

cImageLoader::~cImageLoader() {
}

bool cImageLoader::LoadLogo(const char *logo, int width = -1, int height = -1) {
    if( width == -1 )
        width = logoWidth;
    if( height == -1 )
        height = logoHeight;

    if( (width == 0) || (height==0) )
        return false;
    std::string logoLower = logo;
    toLowerCase(logoLower);
    cString File = cString::sprintf("%s/%s.%s", *Config.logoPath, logoLower.c_str(), *logoExtension);
    bool success = LoadImage(File);
    if( !success ) {
        dsyslog("imageloader LoadLogo: %s could not be loaded", *File);
        return false;
    }

    if( height != 0 || width != 0 ) {
        buffer.sample( Geometry(width, height) );
    }
    return true;
}

int cImageLoader::Height(void) {
    Geometry geo = buffer.size();
    return geo.height();
}

int cImageLoader::Width(void) {
    Geometry geo = buffer.size();
    return geo.width();
}

bool cImageLoader::LoadIcon(const char *cIcon, int size) {
	if (size==0)
        return false;
    cString File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, Setup.OSDTheme, cIcon, *logoExtension);
    bool success = LoadImage(File);
    if( !success ) {
        File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, "default", cIcon, *logoExtension);
        success = LoadImage(File);
        if( !success ) {
            dsyslog("imageloader LoadIcon: %s could not be loaded", *File);
            return false;
        }
    }
    if( size >= 0 )
        buffer.sample(Geometry(size, size));
    return true;
}

bool cImageLoader::LoadIcon(const char *cIcon, int width, int height, bool preserveAspect) {
    try {
        if ((width == 0)||(height==0))
            return false;
        cString File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, Setup.OSDTheme, cIcon, *logoExtension);

        bool success = LoadImage(File);
        if( !success ) {
            File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, "default", cIcon, *logoExtension);
            success = LoadImage(File);
            if( !success ) {
                dsyslog("imageloader LoadIcon: %s could not be loaded", *File);
                return false;
            }
        }
        if (preserveAspect) {
            buffer.sample(Geometry(width, height));
        } else {
            cString geometry = cString::sprintf("%dx%d!", width, height);
            buffer.scale(Geometry(*geometry));
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

cImage cImageLoader::GetImage() {
    int w, h;
    w = buffer.columns();
    h = buffer.rows();
    cImage image (cSize(w, h));
    const PixelPacket *pixels = buffer.getConstPixels(0, 0, w, h);
    for (int iy = 0; iy < h; ++iy) {
        for (int ix = 0; ix < w; ++ix) {
            tColor col = (~int(pixels->opacity * 255 / MaxRGB) << 24) 
            | (int(pixels->green * 255 / MaxRGB) << 8) 
            | (int(pixels->red * 255 / MaxRGB) << 16) 
            | (int(pixels->blue * 255 / MaxRGB) );
            image.SetPixel(cPoint(ix, iy), col);
            ++pixels;
        }
    }
    return image;
}

Color cImageLoader::Argb2Color(tColor col) {
    tIndex alpha = (col & 0xFF000000) >> 24;
    tIndex red = (col & 0x00FF0000) >> 16;
    tIndex green = (col & 0x0000FF00) >> 8;
    tIndex blue = (col & 0x000000FF);
    Color color(MaxRGB*red/255, MaxRGB*green/255, MaxRGB*blue/255, MaxRGB*(0xFF-alpha)/255);
    return color;
}

void cImageLoader::toLowerCase(std::string &str) {
    const int length = str.length();
    for(int i=0; i < length; ++i) {
        str[i] = std::tolower(str[i]);
    }
}

bool cImageLoader::LoadImage(cString File) {
    try {
        buffer.read(*File);
    } catch (...) {
        return false;
    }
    return true;
}
