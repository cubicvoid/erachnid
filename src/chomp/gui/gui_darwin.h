#pragma once

#include <memory>

namespace chomp {

class GUIWrapper;
class Plugin;

GUIWrapper* NewGUIWrapperDarwin(Plugin* plugin);

}  // namespace chomp
