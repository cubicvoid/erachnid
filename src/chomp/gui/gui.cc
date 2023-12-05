#include "gui.hh"

#ifdef DARWIN
#include "gui_darwin.h"
#endif

namespace erachnid::chomp {

ChompGUI* ChompGUI::New(ChompPlugin* plugin) {
  // If we supported more than one platform, this is where we would
  // return the appropriate subclass of ChompGUI.
  return NewChompGUIDarwin(plugin);
}

}  // namespace erachnid::chomp
