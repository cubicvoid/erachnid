#pragma once

#include <memory>

namespace erachnid::chomp {

class ChompGUI;
class Plugin;

ChompGUI* NewChompGUIDarwin(Plugin* plugin);

}  // namespace erachnid::chomp
