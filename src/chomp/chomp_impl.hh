#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace chomp {

struct Voice {};

class Plugin {
 private:
  void ProcessEvent(const clap_event_header_t *hdr);

  int noteOnCount = 0;
  int noteOffCount = 0;
  int midiCount = 0;
  int sampleCount = 0;

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

  uint32_t ParamsCount();
  bool     ParamsGetInfo(uint32_t param_index, clap_param_info_t *param_info);
  bool     ParamsGetValue(clap_id param_id, double *out_value);

  bool ParamsValueToText(
      clap_id  param_id,
      double   value,
      char    *out_buffer,
      uint32_t out_buffer_capacity
  );
  bool ParamsTextToValue(
      clap_id param_id, const char *param_value_text, double *out_value
  );

  void ParamsFlush(
      const clap_input_events_t *in, const clap_output_events_t *out
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
