#include <clap/clap.h>

#include "starry_plugin.hh"

namespace erachnid::starry {

// Needs to be explicitly declared extern so it isn't just linked internally
extern const clap_plugin_descriptor_t plugin_desc;
const clap_plugin_descriptor_t        plugin_desc = {
           .clap_version = CLAP_VERSION_INIT,
           .id = "me.faec.erachnid.starry",
           .name = "erachnid starry",
           .vendor = "cubicvoid",
           .url = "https://faec.me",
           .manual_url = "",
           .support_url = "",
           .version = "0.0.1",
           .description = "additive synthesizer with skewed harmonics",
           .features = (const char *[]
    ){CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

clap_plugin_t *PluginCreate(const clap_host_t *host) {
  StarryPlugin *p = new StarryPlugin(host);
  return p->RawPlugin();
}

}  // namespace erachnid::starry