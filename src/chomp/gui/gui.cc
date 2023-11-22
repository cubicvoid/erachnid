#include "gui.hh"

#include "gui_darwin.h"

namespace chomp {

GUIWrapper* GUIWrapper::New(Plugin* plugin) {
  // If we supported more than one platform, this is where we would
  // return the appropriate subclass of GUIWrapper.
  return NewGUIWrapperDarwin(plugin);
}

GUIWrapper::~GUIWrapper() {}

}  // namespace chomp
