#pragma once

#include <memory>

namespace erachnid::scan {

class Plugin;

Plugin* NewPluginDarwin(const clap_host_t* host);

}  // namespace erachnid::skeleton
