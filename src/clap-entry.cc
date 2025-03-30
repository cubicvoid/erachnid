#include <clap/clap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <concepts>

#include "chomp.hh"
#include "clap_plugin.hh"
#include "skeleton.hh"
#include "starry.hh"

using namespace erachnid;

struct PluginEntry {
  const clap_plugin_descriptor_t *desc;
  clap_plugin_t *(CLAP_ABI *create)(const clap_host_t *host);
};

static PluginEntry s_plugins[] = {
    /*{
        .desc = &s_my_plug_desc,
        .create = my_plug_create,
    },*/
    {
        .desc = &chomp::plugin_desc,
        .create = chomp::PluginCreate,
    },
    {
        .desc = &skeleton::plugin_desc,
        .create = skeleton::PluginCreate,
    },
    {
        .desc = &starry::plugin_desc,
        .create = starry::PluginCreate,
    }
};

static uint32_t plugin_factory_get_plugin_count(
    const struct clap_plugin_factory *factory
) {
  return sizeof(s_plugins) / sizeof(s_plugins[0]);
}

static const clap_plugin_descriptor_t *plugin_factory_get_plugin_descriptor(
    const struct clap_plugin_factory *factory, uint32_t index
) {
  return s_plugins[index].desc;
}

static const clap_plugin_t *plugin_factory_create_plugin(
    const struct clap_plugin_factory *factory,
    const clap_host_t                *host,
    const char                       *plugin_id
) {
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
  if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID)) {
    return &s_plugin_factory;
  }
  return NULL;
}

// This symbol will be resolved by the host
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION_INIT,
    .init = entry_init,
    .deinit = entry_deinit,
    .get_factory = entry_get_factory,
};
