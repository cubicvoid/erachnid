#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

namespace chomp {

uint32_t Plugin::NotePortsCount(bool is_input) {
  flog("chomp_note_ports_count");
  return 1;
}

bool Plugin::NotePortsGet(
    uint32_t index, bool is_input, clap_note_port_info_t *info
) {
  if (index > 0) return false;
  info->id = 0;
  snprintf(info->name, sizeof(info->name), "Trigger");
  info->supported_dialects = CLAP_NOTE_DIALECT_CLAP;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  return true;
}

uint32_t Plugin::AudioPortsCount(bool is_input) {
  flog("chomp_audio_ports_count(is_input: %d)", is_input);
  return 1;
}

bool Plugin::AudioPortsGet(
    uint32_t index, bool is_input, clap_audio_port_info_t *info
) {
  flog("chomp_audio_ports_get(index: %d, is_input: %d)", index, is_input);
  if (index > 0) {
    return false;
  }
  info->id = 0;
  if (is_input) {
    snprintf(info->name, sizeof(info->name), "Input");
  } else {
    snprintf(info->name, sizeof(info->name), "Output");
  }
  info->channel_count = 2;
  info->flags = CLAP_AUDIO_PORT_IS_MAIN;  // | CLAP_AUDIO_PORT_SUPPORTS_64BITS;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}

}  // namespace chomp