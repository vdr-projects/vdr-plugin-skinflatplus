#pragma once
#include <vdr/osd.h>
#include <vdr/skins.h>
#include <string>

#define MAX_IMAGE_CACHE     999

class cImageCache {
private:
    cImage *CacheImage[MAX_IMAGE_CACHE];
    std::string CacheName[MAX_IMAGE_CACHE];
    int CacheWidth[MAX_IMAGE_CACHE];
    int CacheHeight[MAX_IMAGE_CACHE];
    
    int InsertIndex;
public:
    cImageCache();
    ~cImageCache();
    
    void Create(void);
    void Clear(void);
    
    cImage *GetImage(std::string Name, int Width, int Height);
    void InsertImage(cImage *Image, std::string Name, int Width, int Height);
};
