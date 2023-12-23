#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>

#include "clap_gui.hh"
#include "clap_param.hh"
#include "reaper_plugin.h"

namespace erachnid {

class CLAPGUI;

class CLAPPlugin {
 public:
  CLAPPlugin(const clap_host_t *_host, const clap_plugin_descriptor_t *desc);
  virtual ~CLAPPlugin();

  clap_process_status Process(const clap_process_t *process);

  virtual bool Init();
  virtual void Destroy();
  virtual bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );
  virtual void Deactivate();
  virtual void Reset();

  virtual bool StartProcessing() {
    _processing = true;
    return true;
  }
  virtual void StopProcessing() { _processing = false; }

  // If default settings and names are ok, only override NotePortsCount.
  // If names matter, also override NotePortsName.
  virtual uint32_t NotePortsCount(bool is_input);

  // dest points to a buffer of size CLAP_NAME_SIZE
  virtual void NotePortsName(uint32_t index, bool is_input, char *dest);

  // If default settings and names are ok, only override AudioPortsCount.
  // If names matter, also override AudioPortsName.
  virtual uint32_t AudioPortsCount(bool is_input);

  // dest points to a buffer of size CLAP_NAME_SIZE
  virtual void AudioPortsName(uint32_t index, bool is_input, char *dest);

  bool NotePortsEnabled();
  bool NotePortsGet(uint32_t index, bool is_input, clap_note_port_info_t *info);

  bool AudioPortsEnabled();
  bool AudioPortsGet(
      uint32_t index, bool is_input, clap_audio_port_info_t *info
  );

  virtual uint32_t ParamsCount();
  virtual bool     ParamsGetInfo(
          uint32_t param_index, clap_param_info_t *param_info
      );
  virtual bool ParamsGetValue(clap_id param_id, double *out_value);
  virtual bool ParamsValueToText(
      clap_id  param_id,
      double   value,
      char    *out_buffer,
      uint32_t out_buffer_capacity
  );
  virtual bool ParamsTextToValue(
      clap_id param_id, const char *param_value_text, double *out_value
  );
  virtual void ParamsFlush(
      const clap_input_events_t *in, const clap_output_events_t *out
  );

  virtual bool GUIEnabled() { return false; }
  virtual bool GUIIsAPISupported(const char *api, bool is_floating) {
    return false;
  }
  virtual bool GUIGetPreferredAPI(const char **api, bool *is_floating) {
    return false;
  }
  virtual bool GUICreate(const char *api, bool is_floating) { return false; }
  virtual bool GUISetScale(double scale) { return false; }
  virtual void GUIDestroy() {}
  virtual bool GUIGetSize(uint32_t *width, uint32_t *height) { return false; }
  virtual bool GUICanResize() { return false; }
  virtual bool GUIAdjustSize(uint32_t *width, uint32_t *height) {
    return false;
  }
  virtual bool GUISetSize(uint32_t width, uint32_t height) { return false; }
  virtual bool GUISetParent(const clap_window_t *window) { return false; }
  virtual bool GUISetTransient(const clap_window_t *window) { return false; }
  virtual bool GUIShow() { return false; }
  virtual bool GUIHide() { return false; }

  clap_plugin_t *RawPlugin() { return &_raw_plugin; }

  CLAPParam *ParamForID(clap_id id) {
    auto it = _params_lookup.find(id);
    if (it == _params_lookup.end()) {
      return nullptr;
    }
    return it->second;
  }

#ifdef NDEBUG
  void Log(const char *format...) {}
#else
  void Log(const char *format...);
#endif

 protected:
  void AddParam(CLAPParam *param) {
    _params.push_back(param);
    _params_lookup[param->_id] = param;
  };

  const clap_host_t *_host;

  int  _plugin_id;
  bool _active;
  bool _processing;

  // Only valid when _active is true (between calls to Activate and Deactivate)
  double   _sample_rate;
  uint32_t _min_frames_count;
  uint32_t _max_frames_count;

  const reaper_plugin_info_t *_reaper;

 private:
  void InitRawPlugin(const clap_plugin_descriptor_t *desc);

  clap_plugin_t _raw_plugin;

  std::vector<CLAPParam *>       _params;
  std::map<clap_id, CLAPParam *> _params_lookup;
};

}  // namespace erachnid
