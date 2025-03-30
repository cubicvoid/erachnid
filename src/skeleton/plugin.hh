#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_param.hh"
#include "clap_plugin.hh"

namespace erachnid::skeleton {

class Plugin : public CLAPPlugin {
 public:
  Plugin(const clap_host_t *_host);

  virtual clap_process_status Process(const clap_process_t *process);

  virtual uint32_t NotePortsCount(bool is_input) { return 1; }
  virtual uint32_t AudioPortsCount(bool is_input) { return 1; }

  uint32_t latency;

 private:
  void ProcessEvent(const clap_event_header_t *hdr);
  void NoteOn(const clap_event_note_t *note);
  void NoteOff(const clap_event_note_t *note);
};

}  // namespace erachnid::skeleton
