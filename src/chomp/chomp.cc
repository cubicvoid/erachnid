#include <clap/clap.h>

#include "chomp_plugin.hh"

#ifdef DARWIN
#include "gui/chomp_gui_darwin.h"
#endif

namespace erachnid::chomp {

// Needs to be explicitly declared extern so it isn't just linked internally
extern const clap_plugin_descriptor_t plugin_desc;
const clap_plugin_descriptor_t        plugin_desc = {
           .clap_version = CLAP_VERSION_INIT,
           .id = "me.faec.erachnid.chomp",
           .name = "erachnid chomp",
           .vendor = "cubicvoid",
           .url = "https://faec.me",
           .manual_url = "https://your-domain.com/your-plugin/manual",
           .support_url = "https://your-domain.com/support",
           .version = "0.0.1",
           .description = "midi-triggered amp envelopes",
           .features = (const char *[]
    ){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

clap_plugin_t *PluginCreate(const clap_host_t *host) {
  ChompPlugin *p;
#ifdef DARWIN
  p = NewChompPluginDarwin(host);
#endif
  return p->RawPlugin();
}

}  // namespace erachnid::chomp