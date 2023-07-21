#pragma once

#include <clap/clap.h>

clap_plugin_t *chomp_plug_create(const clap_host_t *host);
extern const clap_plugin_descriptor_t chomp_plug_desc;
