#include <clap/clap.h>

#include "plugin.hh"

#ifdef DARWIN
#include "gui/gui_darwin.h"
#endif

namespace erachnid::scan {

// Needs to be explicitly declared extern so it isn't just linked internally
extern const clap_plugin_descriptor_t plugin_desc;
const clap_plugin_descriptor_t        plugin_desc = {
           .clap_version = CLAP_VERSION_INIT,
           .id = "me.faec.erachnid.scan",
           .name = "erachnid scan",
           .vendor = "cubicvoid",
           .url = "https://faec.me",
           .manual_url = "https://faec.me",
           .support_url = "https://faec.me",
           .version = "0.0.1",
           .description = "report daw interactions",
           .features = (const char *[]){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                                        CLAP_PLUGIN_FEATURE_STEREO,
                                        CLAP_PLUGIN_FEATURE_INSTRUMENT, NULL},
};

clap_plugin_t *PluginCreate(const clap_host_t *host) {
  Plugin *p;
#ifdef DARWIN
  p = NewPluginDarwin(host);
#endif
  return p->RawPlugin();
}

}  // namespace erachnid::skeleton