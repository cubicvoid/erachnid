#pragma once

#include <memory>

namespace erachnid::chomp {

class ChompGUI;
class ChompPlugin;

ChompGUI* NewChompGUIDarwin(ChompPlugin* plugin);

}  // namespace erachnid::chomp
