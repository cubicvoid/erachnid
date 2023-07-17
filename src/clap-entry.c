#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <clap/clap.h>

extern const clap_plugin_factory_t s_plugin_factory;

////////////////
// clap_entry //
////////////////

static bool entry_init(const char *plugin_path) {
   // called only once, and very first
   return true;
}

static void entry_deinit(void) {
   // called before unloading the DSO
}

static const void *entry_get_factory(const char *factory_id) {
   if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID))
      return &s_plugin_factory;
   return NULL;
}

// This symbol will be resolved by the host
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
   .clap_version = CLAP_VERSION_INIT,
   .init = entry_init,
   .deinit = entry_deinit,
   .get_factory = entry_get_factory,
};
