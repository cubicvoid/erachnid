#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_param.hh"
#include "clap_plugin.hh"

namespace erachnid::chomp {

// Abstract class for Chomp (concrete implementations for each GUI
// platform, which so far is just darwin)
class ChompPlugin : public CLAPPlugin {
 public:
  ChompPlugin(const clap_host_t *_host);

  virtual clap_process_status Process(const clap_process_t *process);

  virtual uint32_t NotePortsCount(bool is_input) { return 1; }
  virtual uint32_t AudioPortsCount(bool is_input) { return 1; }

  uint32_t latency;

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
};

}  // namespace erachnid::chomp
