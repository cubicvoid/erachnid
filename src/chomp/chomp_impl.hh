#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "params.hh"

namespace chomp {

class GUIWrapper;

struct Voice {};

class Plugin {
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

  uint32_t ParamCount();
  bool     ParamGetInfo(uint32_t param_index, clap_param_info_t *param_info);
  bool     ParamGetValue(clap_id param_id, double *out_value);
  bool     ParamValueToText(
          clap_id  param_id,
          double   value,
          char    *out_buffer,
          uint32_t out_buffer_capacity
      );
  bool ParamTextToValue(
      clap_id param_id, const char *param_value_text, double *out_value
  );
  void ParamFlush(
      const clap_input_events_t *in, const clap_output_events_t *out
  );
  // FILE *logFile;

  void flog(const char *format...);

  clap_plugin_t                   plugin;
  const clap_host_t              *host;
  const clap_host_latency_t      *host_latency;
  const clap_host_thread_check_t *host_thread_check;
  const clap_host_state_t        *host_state;

  std::unique_ptr<GUIWrapper> gui;
  // const clap_window_t        *window;

  void (*log)(const clap_host_t *host, int pluginID, const char *msg);

  uint32_t latency;

  int  pluginID;
  bool active;
  bool processing;

 protected:
  struct Param {
    Param(clap_param_info_t _info, double _value)
        : info(_info), value(_value) {}
    clap_param_info_t info;
    double            value;
  };

  void AddParam(
      ParamID     id,
      std::string name,
      std::string module,
      double      minValue,
      double      maxValue,
      double      defaultValue,
      uint64_t    flags
  );

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

  void RefreshParameters();

  std::vector<Param *> paramsAll;
  std::vector<Param *> paramsActive;
};

}  // namespace chomp
