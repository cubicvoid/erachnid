#pragma once

#include <memory>

namespace erachnid::skeleton {

class Plugin;

Plugin* NewPluginDarwin(const clap_host_t* host);

}  // namespace erachnid::skeleton
