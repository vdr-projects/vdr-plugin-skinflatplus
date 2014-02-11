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

cImage* cImageLoader::LoadLogo(const char *logo, int width, int height) {
    if( (width == 0) || (height==0) )
        return NULL;
    dsyslog("imageloader LoadLogo: %s", logo);
    std::string logoLower = logo;
    toLowerCase(logoLower);
    cString File = cString::sprintf("%s/%s.%s", *Config.logoPath, logoLower.c_str(), *logoExtension);

    cImage *img;
    img = imgCache.GetImage( *File, width, height );
    if( img != NULL )
        return img;

    bool success = LoadImage(File);
    if( !success ) {
        dsyslog("imageloader LoadLogo: %s could not be loaded", *File);
        return NULL;
    }

    img = CreateImage(width, height);
    if( img == NULL )
        return NULL;
    imgCache.InsertImage(img, logoLower, width, height);
    return img;
}

cImage* cImageLoader::LoadIcon(const char *cIcon, int width, int height, bool preserveAspect) {
    if ((width == 0)||(height==0))
        return NULL;

    cString File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, Setup.OSDTheme, cIcon, *logoExtension);

    cImage *img;
    img = imgCache.GetImage( *File, width, height );
    if( img != NULL )
        return img;

    bool success = LoadImage(File);
    if( !success ) {
        File = cString::sprintf("%s%s/%s.%s", *Config.iconPath, "default", cIcon, *logoExtension);
        img = imgCache.GetImage( *File, width, height );
        if( img != NULL )
            return img;

        success = LoadImage(File);
        if( !success ) {
            dsyslog("imageloader LoadIcon: %s could not be loaded", *File);
            return NULL;
        }
    }
    img = CreateImage(width, height);
    if( img == NULL )
        return NULL;
    imgCache.InsertImage(img, cIcon, width, height);
    return img;
}

/*
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
*/

/*
bool cImageLoader::LoadIcon2(const char *cIcon) {
    try {
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
        return true;
    }
    catch (...) {
        return false;
    }
}

cImage cImageLoader::GetImage() {
    return CreateImageCopy();
}

cImage cImageLoader::GetImage(int width, int height, bool preserveAspect) {
    cImage *img;
    img = CreateImage(width, height, preserveAspect);
    return *img;
}
*/

void cImageLoader::toLowerCase(std::string &str) {
    const int length = str.length();
    for(int i=0; i < length; ++i) {
        str[i] = std::tolower(str[i]);
    }
}
