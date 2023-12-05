#pragma once

#include <clap/clap.h>

#include "clap_gui.hh"

namespace erachnid::chomp {

class Plugin;

class ChompGUI : public CLAPGUI {
 public:
  static ChompGUI *New(Plugin *plugin);
};

}  // namespace erachnid::chomp
