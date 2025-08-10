#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_param.hh"
#include "clap_plugin.hh"

namespace erachnid::scan {

struct SerializedEvent {
  uint32_t time;
  uint16_t space_id;
  uint16_t type;
};

struct SerializedProcessCall {
  uint32_t frames_count;
  std::vector<SerializedEvent> events;
};
  
  
class Plugin : public CLAPPlugin {
 public:
  Plugin(const clap_host_t *_host);

  virtual clap_process_status Process(const clap_process_t *process);

  virtual uint32_t NotePortsCount(bool is_input) { return 1; }
  virtual uint32_t AudioPortsCount(bool is_input) { return 1; }

  uint32_t latency;

 private:
  void NoteOn(const clap_event_note_t *note);
  void NoteOff(const clap_event_note_t *note);

  std::vector<SerializedProcessCall> processCalls;
};

}  // namespace erachnid::skeleton
