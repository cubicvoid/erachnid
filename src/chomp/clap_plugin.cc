#include "clap_plugin.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

namespace erachnid {

CLAPPlugin::CLAPPlugin(const clap_host_t *_host)
    : processing(false), active(false) {
  static int count = 0;
  pluginID = count++;

  init_plugin(&plugin, reinterpret_cast<void *>(this));
}

bool CLAPPlugin::Init(const clap_host_t *host) { return true; }

void CLAPPlugin::Destroy(const clap_host_t *host) {}

bool CLAPPlugin::Activate(
    const clap_host_t *host,
    double             sample_rate,
    uint32_t           min_frames_count,
    uint32_t           max_frames_count
) {
  Log("chomp_plug_activate(%lf, %d, %d)", sample_rate, min_frames_count,
      max_frames_count);
  return true;
}

void CLAPPlugin::Deactivate(const clap_host_t *host) {
  Log("chomp_plug_deactivate (%d note on, %d note off, %d midi, %d samples)",
      noteOnCount, noteOffCount, midiCount, sampleCount);
}

void CLAPPlugin::Reset(const clap_host_t *host) {}

}  // namespace erachnid
