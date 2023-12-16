#pragma once

#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_param.hh"
#include "clap_plugin.hh"

namespace erachnid::starry {

class StarryPlugin : public CLAPPlugin {
 public:
  StarryPlugin(const clap_host_t *_host);

  virtual clap_process_status Process(const clap_process_t *process);

  virtual bool Init();
  virtual void Destroy();
  virtual void Deactivate();
  virtual bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );
  virtual bool StartProcessing();
  virtual void StopProcessing();
  virtual void Reset();

  virtual uint32_t NotePortsCount(bool is_input) { return is_input ? 1 : 0; };
  virtual uint32_t AudioPortsCount(bool is_input) { return is_input ? 0 : 1; };

  uint32_t latency;

  int pluginID;

 private:
  void ProcessEvent(const clap_event_header_t *hdr);
  void NoteOn(const clap_event_note_t *note);
  void NoteOff(const clap_event_note_t *note);
};

}  // namespace erachnid::starry
