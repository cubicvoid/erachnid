#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_gui.hh"

namespace erachnid {

class CLAPGUI;

class CLAPPlugin {
 public:
  CLAPPlugin(const clap_host_t *_host);

  clap_process_status Process(const clap_process_t *process);

  virtual bool Init() = 0;
  virtual void Destroy() = 0;
  virtual bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  ) = 0;
  virtual void Deactivate() = 0;
  virtual void Reset() = 0;

  virtual bool StartProcessing() { processing = true; }
  virtual void StopProcessing() { processing = false; }

  virtual uint32_t NotePortsCount(bool is_input);
  virtual bool     NotePortsGet(
          uint32_t index, bool is_input, clap_note_port_info_t *info
      );

  virtual uint32_t AudioPortsCount(bool is_input);
  virtual bool     AudioPortsGet(
          uint32_t index, bool is_input, clap_audio_port_info_t *info
      );

  virtual uint32_t ParamCount();
  virtual bool     ParamGetInfo(
          uint32_t param_index, clap_param_info_t *param_info
      );
  virtual bool ParamGetValue(clap_id param_id, double *out_value);
  virtual bool ParamValueToText(
      clap_id  param_id,
      double   value,
      char    *out_buffer,
      uint32_t out_buffer_capacity
  );
  virtual bool ParamTextToValue(
      clap_id param_id, const char *param_value_text, double *out_value
  );
  virtual void ParamFlush(
      const clap_input_events_t *in, const clap_output_events_t *out
  );
  clap_plugin_t plugin;

  bool GUIIsAPISupported(const char *api, bool is_floating) {
    return gui->IsAPISupported(api, is_floating);
  }
  bool GUIGetPreferredAPI(const char **api, bool *is_floating) {
    return gui->GetPreferredAPI(api, is_floating);
  }
  bool GUICreate(const char *api, bool is_floating) {
    return gui->Create(api, is_floating);
  }
  bool GUISetScale(double scale) { return gui->SetScale(scale); }
  void GUIDestroy() { return gui->Destroy(); }
  bool GUIGetSize(uint32_t *width, uint32_t *height) {
    return gui->GetSize(width, height);
  }
  bool GUICanResize() { return gui->CanResize(); }
  bool GUIAdjustSize(uint32_t *width, uint32_t *height) {
    return gui->AdjustSize(width, height);
  }
  bool GUISetSize(uint32_t width, uint32_t height) {
    return gui->SetSize(width, height);
  }
  bool GUISetParent(const clap_window_t *window) {
    return gui->SetParent(window);
  }
  bool GUISetTransient(const clap_window_t *window) {
    return gui->SetTransient(window);
  }
  bool GUIShow() { return gui->Show(); }
  bool GUIHide() { return gui->Hide(); }

 protected:
  const clap_host_t *host;

  int  pluginID;
  bool active;
  bool processing;

  std::unique_ptr<CLAPGUI> gui;
};

}  // namespace erachnid
