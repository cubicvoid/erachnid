#include <clap/clap.h>

#include "starry_plugin.hh"

#ifdef DARWIN
// #include "gui/starry_gui_darwin.h"
#endif

namespace erachnid::starry {

// Needs to be explicitly declared extern so it isn't just linked internally
extern const clap_plugin_descriptor_t plugin_desc;
const clap_plugin_descriptor_t        plugin_desc = {
           .clap_version = CLAP_VERSION_INIT,
           .id = "me.faec.erachnid.starry",
           .name = "erachnid starry",
           .vendor = "cubicvoid",
           .url = "https://faec.me",
           .manual_url = "https://faec.me",
           .support_url = "https://faec.me",
           .version = "0.0.1",
           .description = "additive synthesizer with skewed harmonics",
           .features = (const char *[]
    ){CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

clap_plugin_t *PluginCreate(const clap_host_t *host) {
  /*ChompPlugin *p;
#ifdef DARWIN
  p = NewChompPluginDarwin(host);
#endif
  return p->RawPlugin();*/
  return NULL;
}

}  // namespace erachnid::starry