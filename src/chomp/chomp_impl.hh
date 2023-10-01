#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "params.hh"

namespace chomp {

struct Voice {};

class Plugin {
 private:
  void ProcessEvent(const clap_event_header_t *hdr);
  void NoteOn(const clap_event_note_t *note);
  void NoteOff(const clap_event_note_t *note);

  int noteOnCount = 0;
  int noteOffCount = 0;
  int midiCount = 0;
  int sampleCount = 0;

  double envVolume = 0.0;
  double envTarget = 0.0;

  Param rats = Param(
      ParamIDRats,
      "rats",
      "something",
      0,
      100,
      50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  );
  Param attack = Param(
      ParamIDAttack,
      "attack",
      "something else",
      0.0,
      1.0,
      0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  );

 public:
  Plugin(const clap_host_t *_host);

  clap_process_status Process(const clap_process_t *process);

  bool Init();
  void Destroy();
  void Deactivate();
  bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );
  bool StartProcessing();
  void StopProcessing();
  void Reset();

  uint32_t NotePortsCount(bool is_input);
  bool NotePortsGet(uint32_t index, bool is_input, clap_note_port_info_t *info);

  uint32_t AudioPortsCount(bool is_input);
  bool     AudioPortsGet(
          uint32_t index, bool is_input, clap_audio_port_info_t *info
      );

  FILE *logFile;

  void flog(const clap_host_t *host, clap_log_severity sev, const char *msg) {
    if (logFile != nullptr) {
      struct timeval tv;
      gettimeofday(&tv, nullptr);

      char buf[512];
      snprintf(
          buf, sizeof(buf), "(%d) %ld.%lf %s\n", sev, tv.tv_sec,
          static_cast<double>(tv.tv_usec) / 1000000.0, msg
      );
      fwrite(buf, 1, strlen(buf), logFile);
      fflush(logFile);
    }
  }

  clap_plugin_t                   plugin;
  const clap_host_t              *host;
  const clap_host_latency_t      *host_latency;
  const clap_host_thread_check_t *host_thread_check;
  const clap_host_state_t        *host_state;

  void(CLAP_ABI *log)(
      const clap_host_t *host, clap_log_severity severity, const char *msg
  );

  uint32_t latency;

  bool active;
  bool processing;
};

}  // namespace chomp
