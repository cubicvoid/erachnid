#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <clap/clap.h>

#include "example.h"
#include "chomp.hh"

/////////////////////////
// clap_plugin_factory //
/////////////////////////

typedef struct {
   const clap_plugin_descriptor_t *desc;
   clap_plugin_t *(CLAP_ABI *create)(const clap_host_t *host);
} PluginEntry;

static PluginEntry s_plugins[] = {
   {
      .desc = &s_my_plug_desc,
      .create = my_plug_create,
   },
   {
      .desc = &chomp_plug_desc,
      .create = chomp_plug_create,
   },
};

static uint32_t plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) {
   return sizeof(s_plugins) / sizeof(s_plugins[0]);
}

static const clap_plugin_descriptor_t *
plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory, uint32_t index) {
   return s_plugins[index].desc;
}

static const clap_plugin_t *plugin_factory_create_plugin(const struct clap_plugin_factory *factory,
                                                         const clap_host_t                *host,
                                                         const char *plugin_id) {
   if (!clap_version_is_compatible(host->clap_version)) {
      return NULL;
   }

   const int N = sizeof(s_plugins) / sizeof(s_plugins[0]);
   for (int i = 0; i < N; ++i)
      if (!strcmp(plugin_id, s_plugins[i].desc->id))
         return s_plugins[i].create(host);

   return NULL;
}

const clap_plugin_factory_t s_plugin_factory = {
   .get_plugin_count = plugin_factory_get_plugin_count,
   .get_plugin_descriptor = plugin_factory_get_plugin_descriptor,
   .create_plugin = plugin_factory_create_plugin,
};

