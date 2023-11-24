#pragma once

#include <memory>

namespace erachnid::chomp {

class GUIWrapper;
class Plugin;

GUIWrapper* NewGUIWrapperDarwin(Plugin* plugin);

}  // namespace erachnid::chomp
