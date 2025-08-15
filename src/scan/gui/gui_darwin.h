#pragma once

#include <memory>

namespace erachnid {

class CLAPPlugin;

}

namespace erachnid::scan {

CLAPPlugin* NewPluginDarwin(const clap_host_t* host);

}  // namespace erachnid::skeleton
