#include "imagecache.h"

cImageCache::cImageCache() {

}

cImageCache::~cImageCache() {
    
}

void cImageCache::Create(void) {
    for(int i = 0; i < MAX_IMAGE_CACHE; i++) {
        CacheImage[i] = NULL;
        CacheName[i] = "";
        CacheWidth[i] = -1;
        CacheHeight[i] = -1;
    }

    InsertIndex = 0;
}

void cImageCache::Clear(void) {
    for(int i = 0; i < MAX_IMAGE_CACHE; i++) {
        if( CacheImage[i] != NULL )
            delete CacheImage[i];
    }

    InsertIndex = 0;
}

cImage* cImageCache::GetImage(std::string Name, int Width, int Height) {
    for(int index = 0; index < MAX_IMAGE_CACHE; index++ ) {
        if( CacheName[index] == Name && CacheWidth[index] == Width && CacheHeight[index] == Height )
            return CacheImage[index];
    }
    return NULL;
}

void cImageCache::InsertImage(cImage *Image, std::string Name, int Width, int Height) {
    CacheImage[InsertIndex] = Image;
    CacheName[InsertIndex] = Name;
    CacheWidth[InsertIndex] = Width;
    CacheHeight[InsertIndex] = Height;
    
    dsyslog("imagecache InsertImage");

    InsertIndex++;
    if( InsertIndex >= MAX_IMAGE_CACHE ) {
        InsertIndex = 0;
        dsyslog("imagecache overflow");
    }
}
