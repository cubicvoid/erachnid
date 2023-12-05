#pragma once

#include <clap/clap.h>

#include "clap_gui.hh"

namespace erachnid::chomp {

class ChompPlugin;

class ChompGUI : public CLAPGUI {
 public:
  static ChompGUI *New(ChompPlugin *plugin);
};

}  // namespace erachnid::chomp
