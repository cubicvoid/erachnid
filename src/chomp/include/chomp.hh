#pragma once

#include <clap/clap.h>

namespace erachnid::chomp {

clap_plugin_t *PluginCreate(const clap_host_t *host);

extern const clap_plugin_descriptor_t plugin_desc;

}  // namespace erachnid::chomp
