#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////

static uint32_t chomp_audio_ports_count(
    const clap_plugin_t *_plugin, bool is_input
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_audio_ports_count(is_input: %d)", is_input);
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  // We just declare 1 audio input and 1 audio output
  return 1;
}

static bool chomp_audio_ports_get(
    const clap_plugin_t    *_plugin,
    uint32_t                index,
    bool                    is_input,
    clap_audio_port_info_t *info
) {
  char buf[256];
  snprintf(
      buf, sizeof(buf), "chomp_audio_ports_get(index: %d, is_input: %d)", index,
      is_input
  );
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  if(index > 0) {
    return false;
  }
  info->id = 0;
  if(is_input) {
    snprintf(info->name, sizeof(info->name), "Input");
  } else {
    snprintf(info->name, sizeof(info->name), "Output");
  }
  info->channel_count = 2;
  info->flags = CLAP_AUDIO_PORT_IS_MAIN | CLAP_AUDIO_PORT_SUPPORTS_64BITS;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}

const clap_plugin_audio_ports_t s_chomp_audio_ports = {
    .count = chomp_audio_ports_count,
    .get = chomp_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t chomp_note_ports_count(
    const clap_plugin_t *plugin, bool is_input
) {
  // We just declare 1 note input
  return 1;
}

static bool chomp_note_ports_get(
    const clap_plugin_t   *plugin,
    uint32_t               index,
    bool                   is_input,
    clap_note_port_info_t *info
) {
  if(index > 0) return false;
  info->id = 0;
  snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
  info->supported_dialects = CLAP_NOTE_DIALECT_CLAP |
                             CLAP_NOTE_DIALECT_MIDI_MPE |
                             CLAP_NOTE_DIALECT_MIDI2;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  return true;
}

static const clap_plugin_note_ports_t s_chomp_note_ports = {
    .count = chomp_note_ports_count,
    .get = chomp_note_ports_get,
};