#import <AppKit/AppKit.h>


#import "gui.h"

#import "gui_darwin.h"


namespace chomp {

GUIWrapper* GUIWrapper::New(Plugin *plugin) {
    // If we supported more than one platform, this is where we would
    // return the appropriate subclass of GUIWrapper.
    return NewGUIWrapperDarwin(plugin);
}

GUIWrapper::~GUIWrapper() { }

}

