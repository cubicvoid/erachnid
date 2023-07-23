#pragma once

#include <clap/clap.h>

namespace chomp {

clap_plugin_t *plugin_create(const clap_host_t *host);

extern const clap_plugin_descriptor_t plugin_desc;

}  // namespace chomp
