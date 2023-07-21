#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <clap/clap.h>

extern clap_plugin_descriptor_t s_my_plug_desc;
extern clap_plugin_t *my_plug_create(const clap_host_t *host);

#ifdef __cplusplus
}
#endif
