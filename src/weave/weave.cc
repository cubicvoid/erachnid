#include <clap/clap.h>

#include "weave_plugin.hh"

#ifdef DARWIN
// #include "gui/weave_gui_darwin.h"
#endif

namespace erachnid::weave {

// Needs to be explicitly declared extern so it isn't just linked internally
extern const clap_plugin_descriptor_t plugin_desc;
const clap_plugin_descriptor_t        plugin_desc = {
           .clap_version = CLAP_VERSION_INIT,
           .id = "me.faec.erachnid.weave",
           .name = "erachnid weave",
           .vendor = "cubicvoid",
           .url = "https://faec.me",
           .manual_url = "https://faec.me",
           .support_url = "https://faec.me",
           .version = "0.0.1",
           .description = "perform step-sequenced midi clips",
           .features = (const char *[]){CLAP_PLUGIN_FEATURE_NOTE_EFFECT, NULL},
};

clap_plugin_t *PluginCreate(const clap_host_t *host) {
  /*ChompPlugin *p;
#ifdef DARWIN
  p = NewChompPluginDarwin(host);
#endif
  return p->RawPlugin();*/
  return NULL;
}

}  // namespace erachnid::weave