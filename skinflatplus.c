/*
 * skinflat.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */
#include <getopt.h>
#include <vdr/plugin.h>
#include <Magick++.h>

#if defined(APIVERSNUM) && APIVERSNUM < 10734
#error "VDR-1.7.34 API version or greater is required!"
#endif

#include "flat.h"
#include "setup.h"
#include "imageloader.h"

static const char *VERSION        = "0.6.0";
static const char *DESCRIPTION    = "skin flatplus";

class cPluginFlat : public cPlugin {
    private:
        cFlat *flat;
    public:
        cPluginFlat(void);
        virtual ~cPluginFlat();
        virtual const char *Version(void) { return VERSION; }
        virtual const char *Description(void) { return DESCRIPTION; }
        virtual const char *CommandLineHelp(void);
        virtual bool ProcessArgs(int argc, char *argv[]);
        virtual bool Initialize(void);
        virtual bool Start(void);
        virtual void Stop(void);
        virtual void Housekeeping(void);
        virtual void MainThreadHook(void);
        virtual cString Active(void);
        virtual time_t WakeupTime(void);
        virtual const char *MainMenuEntry(void) {return NULL;}
        virtual cOsdObject *MainMenuAction(void);
        virtual cMenuSetupPage *SetupMenu(void);
        virtual bool SetupParse(const char *Name, const char *Value);
        virtual bool Service(const char *Id, void *Data = NULL);
        virtual const char **SVDRPHelpPages(void);
        virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
};

cPluginFlat::cPluginFlat(void) {
    flat = NULL;
}

cPluginFlat::~cPluginFlat() {
}

const char *cPluginFlat::CommandLineHelp(void) {
    return "  -l <LOGOPATH>, --logopath=<LOGOPATH>       Set directory where Channel Logos are stored.\n";
}

bool cPluginFlat::ProcessArgs(int argc, char *argv[]) {
    // Implement command line argument processing here if applicable.
    static const struct option long_options[] = {
        { "logopath", required_argument, NULL, 'l' },
        { NULL }
    };

    int c;
    while ((c = getopt_long(argc, argv, "l:", long_options, NULL)) != -1) {
        switch (c) {
            case 'l':
                Config.SetLogoPath(cString(optarg));
                break;
            default:
                return false;
        }
    }
    return true;
}

__attribute__((constructor)) static void init(void) {
   Magick::InitializeMagick(NULL);
}


bool cPluginFlat::Initialize(void) {
    Config.Init();
    return true;
}

bool cPluginFlat::Start(void) {
    if (!cOsdProvider::SupportsTrueColor()) {
        esyslog("skinflatplus: No TrueColor OSD found! Aborting!");
        return false;
    } else
        dsyslog("skinflatplus: TrueColor OSD found");

    imgCache.Create();
    imgCache.PreLoadImage();

    flat = new cFlat;
    return flat;
}

void cPluginFlat::Stop(void) {
    imgCache.Clear();
}

void cPluginFlat::Housekeeping(void) {
}

void cPluginFlat::MainThreadHook(void) {
}

cString cPluginFlat::Active(void) {
    return NULL;
}

time_t cPluginFlat::WakeupTime(void) {
    return 0;
}

cOsdObject *cPluginFlat::MainMenuAction(void) {
    return NULL;
}

cMenuSetupPage *cPluginFlat::SetupMenu(void) {
    return new cFlatSetup();
}

bool cPluginFlat::SetupParse(const char *Name, const char *Value) {
    return Config.SetupParse(Name, Value);
}

bool cPluginFlat::Service(const char *Id, void *Data) {
    return false;
}

const char **cPluginFlat::SVDRPHelpPages(void) {
    static const char *HelpPages[] = {
        "RLFC\n"
        "    Remove Logo From Cache.\n"
        "    Specify logo filename with extension, but without path",
        NULL
    };
    return HelpPages;
}

cString cPluginFlat::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode) {
    if( !strcasecmp(Command, "RLFC")) {
        if( Option == NULL ) {
            ReplyCode = 500;
            return "no logo given";
        }
        if( !strcmp(Option, "") ) {
            ReplyCode = 500;
            return "no logo given";
        }

        if( imgCache.RemoveFromCache( Option ) ) {
            ReplyCode = 900;
            return "successfully remove logo from cache";
        } else {
            ReplyCode = 501;
            return "Failure - logo not found in cache";
        }
    }
    return NULL;
}

VDRPLUGINCREATOR(cPluginFlat); // Don't touch this!
