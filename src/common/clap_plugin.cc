#include "clap_plugin.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

namespace erachnid {

CLAPPlugin::CLAPPlugin(
    const clap_host_t *_host, const clap_plugin_descriptor_t *desc
)
    : host(_host), active(false), processing(false) {
  InitRawPlugin(desc);

  // this is a hacky way of assigning a unique id to each plugin
  // instance, but it's only used for debug logging:
  static int count = 0;
  pluginID = count++;
}

CLAPPlugin::~CLAPPlugin() {}

clap_process_status erachnid::CLAPPlugin::Process(clap_process const *) {}

bool CLAPPlugin::Init() { return true; }

void CLAPPlugin::Destroy() {}

bool CLAPPlugin::Activate(
    double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  return true;
}

void CLAPPlugin::Deactivate() {}

void CLAPPlugin::Reset() {}

}  // namespace erachnid
