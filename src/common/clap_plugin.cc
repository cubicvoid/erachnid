#include "clap_plugin.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

static FILE *logFile = nullptr;

namespace erachnid {

CLAPPlugin::CLAPPlugin(
    const clap_host_t *host, const clap_plugin_descriptor_t *desc
)
    : _host(host), _active(false), _processing(false) {
#ifndef NDEBUG
  // this is a hacky way of assigning a unique id to each plugin
  // instance, but it's only used for debug logging:
  static int count = 0;
  _pluginID = count++;
  if (logFile == nullptr) {
    logFile = fopen("/Users/fae/chomp.log", "wb");
  }
#endif

  InitRawPlugin(desc);
}

CLAPPlugin::~CLAPPlugin() {}

clap_process_status erachnid::CLAPPlugin::Process(clap_process const *) {
  return CLAP_PROCESS_CONTINUE;
}

bool CLAPPlugin::Init() {
  _reaper = reinterpret_cast<const reaper_plugin_info_t *>(
      _host->get_extension(_host, "cockos.reaper_extension")
  );
  return true;
}

void CLAPPlugin::Destroy() {}

bool CLAPPlugin::Activate(
    double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  _sample_rate = sample_rate;
  _min_frames_count = min_frames_count;
  _max_frames_count = max_frames_count;
  _active = true;
  return true;
}

void CLAPPlugin::Deactivate() { _active = false; }

void CLAPPlugin::Reset() {}

bool CLAPPlugin::NotePortsEnabled() {
  return (NotePortsCount(false) > 0 || NotePortsCount(true) > 0);
}

uint32_t CLAPPlugin::NotePortsCount(bool is_input) { return 0; }

static void defaultPortsName(
    uint32_t index, uint32_t total, bool is_input, char *dest
) {
  if (is_input) {
    if (total == 1) {
      snprintf(dest, CLAP_NAME_SIZE, "Input");
    } else {
      snprintf(dest, CLAP_NAME_SIZE, "Input %d", index + 1);
    }
  } else {
    if (total == 1) {
      snprintf(dest, CLAP_NAME_SIZE, "Output");
    } else {
      snprintf(dest, CLAP_NAME_SIZE, "Output %d", index + 1);
    }
  }
}

void CLAPPlugin::NotePortsName(uint32_t index, bool is_input, char *dest) {
  defaultPortsName(index, NotePortsCount(is_input), is_input, dest);
}

bool CLAPPlugin::NotePortsGet(
    uint32_t index, bool is_input, clap_note_port_info_t *info
) {
  if (index >= NotePortsCount(is_input)) return false;
  info->id = index;
  NotePortsName(index, is_input, info->name);
  info->supported_dialects = CLAP_NOTE_DIALECT_CLAP;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  return true;
}

bool CLAPPlugin::AudioPortsEnabled() {
  return (AudioPortsCount(false) > 0 || AudioPortsCount(true) > 0);
}

uint32_t CLAPPlugin::AudioPortsCount(bool is_input) { return 0; }

void CLAPPlugin::AudioPortsName(uint32_t index, bool is_input, char *dest) {
  defaultPortsName(index, AudioPortsCount(is_input), is_input, dest);
}

bool CLAPPlugin::AudioPortsGet(
    uint32_t index, bool is_input, clap_audio_port_info_t *info
) {
  if (index >= AudioPortsCount(is_input)) {
    return false;
  }
  info->id = index;
  AudioPortsName(index, is_input, info->name);
  info->flags = 0;  // CLAP_AUDIO_PORT_SUPPORTS_64BITS;
  if (index == 0) {
    info->flags |= CLAP_AUDIO_PORT_IS_MAIN;
  }
  info->channel_count = 2;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}

#ifndef NDEBUG
void CLAPPlugin::Log(const char *format...) {
  if (logFile != nullptr) {
    va_list args;
    va_start(args, format);

    char timeStr[64];
    int  msec;

    struct timeval tv;
    // time_t         t = static_cast<time_t>(tv.tv_sec);
    struct tm *timeinfo;
    gettimeofday(&tv, nullptr);
    timeinfo = localtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%F %T", timeinfo);
    msec = static_cast<int>(tv.tv_usec / 1000);

    char buf[256];
    snprintf(
        buf, sizeof(buf), "[%d] %s.%03d %s\n", _pluginID, timeStr, msec, format
    );
    vfprintf(logFile, buf, args);
    fflush(logFile);
  }
}
#endif

}  // namespace erachnid
