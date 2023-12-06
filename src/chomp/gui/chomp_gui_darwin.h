#pragma once

#include <memory>

namespace erachnid::chomp {

// class ChompGUI;
class ChompPlugin;

// ChompGUI* NewChompGUIDarwin(ChompPlugin* plugin);
ChompPlugin* NewChompPluginDarwin(const clap_host_t* host);

}  // namespace erachnid::chomp
