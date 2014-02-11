#include <vdr/osd.h>
#include <vdr/menu.h>

#include "flat.h"

#include "displaychannel.h"
#include "displaymenu.h"
#include "displaymessage.h"
#include "displayreplay.h"
#include "displaytracks.h"
#include "displayvolume.h"

class cFlatConfig Config;
class cImageCache imgCache;

cTheme Theme;
static bool menuActive = false;
bool firstDisplay = true;

cFlat::cFlat(void) : cSkin("flatPlus", &::Theme) {
    displayMenu = NULL;
}

const char *cFlat::Description(void) {
    return "flatPlus";
}

cSkinDisplayChannel *cFlat::DisplayChannel(bool WithInfo) {
    return new cFlatDisplayChannel(WithInfo);
}

cSkinDisplayMenu *cFlat::DisplayMenu(void) {
    cFlatDisplayMenu *menu = new cFlatDisplayMenu;
    displayMenu = menu;
    menuActive = true;
    return menu;
}

cSkinDisplayReplay *cFlat::DisplayReplay(bool ModeOnly) {
    return new cFlatDisplayReplay(ModeOnly);
}

cSkinDisplayVolume *cFlat::DisplayVolume(void) {
    return new cFlatDisplayVolume;
}

cSkinDisplayTracks *cFlat::DisplayTracks(const char *Title, int NumTracks, const char * const *Tracks) {
    return new cFlatDisplayTracks(Title, NumTracks, Tracks);
}

cSkinDisplayMessage *cFlat::DisplayMessage(void) {
    return new cFlatDisplayMessage;
}
