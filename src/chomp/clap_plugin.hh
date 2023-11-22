#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "params.hh"

namespace erachnid {

class CLAPPlugin {
 public:
  CLAPPlugin(const clap_host_t *_host);

  clap_process_status Process(const clap_process_t *process);

  virtual bool Init(const clap_host_t *host);
  virtual void Destroy(const clap_host_t *host);
  virtual void Deactivate(const clap_host_t *host);
  virtual bool Activate(
      const clap_host_t *host,
      double             sample_rate,
      uint32_t           min_frames_count,
      uint32_t           max_frames_count
  );
  virtual bool StartProcessing(const clap_host_t *host);
  virtual void StopProcessing(const clap_host_t *host);
  virtual void Reset(const clap_host_t *host);

  virtual uint32_t NotePortsCount(const clap_host_t *host, bool is_input);
  virtual bool     NotePortsGet(
          const clap_host_t     *host,
          uint32_t               index,
          bool                   is_input,
          clap_note_port_info_t *info
      );

  virtual uint32_t AudioPortsCount(const clap_host_t *host, bool is_input);
  virtual bool     AudioPortsGet(
          const clap_host_t      *host,
          uint32_t                index,
          bool                    is_input,
          clap_audio_port_info_t *info
      );

  virtual uint32_t ParamCount(const clap_host_t *host);
  virtual bool     ParamGetInfo(
          const clap_host_t *host,
          uint32_t           param_index,
          clap_param_info_t *param_info
      );
  virtual bool ParamGetValue(
      const clap_host_t *host, clap_id param_id, double *out_value
  );
  virtual bool ParamValueToText(
      const clap_host_t *host,
      clap_id            param_id,
      double             value,
      char              *out_buffer,
      uint32_t           out_buffer_capacity
  );
  virtual bool ParamTextToValue(
      const clap_host_t *host,
      clap_id            param_id,
      const char        *param_value_text,
      double            *out_value
  );
  virtual void ParamFlush(
      const clap_host_t          *host,
      const clap_input_events_t  *in,
      const clap_output_events_t *out
  );
  clap_plugin_t plugin;

  // std::unique_ptr<GUIWrapper> gui;

 protected:
  int  pluginID;
  bool active;
  bool processing;
};

}  // namespace erachnid
