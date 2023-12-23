#pragma once

#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <array>
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

  virtual uint32_t NotePortsCount(bool is_input) { return is_input ? 1 : 0; };
  virtual uint32_t AudioPortsCount(bool is_input) { return is_input ? 0 : 1; };

  virtual clap_process_status Process(const clap_process_t *process);

  void ProcessEvent(const clap_event_header_t *hdr);

 private:
  void handleNoteOn(int port_index, int channel, int key, int noteid);
  void handleNoteOff(int port_index, int channel, int n);

  std::array<StarryVoice, max_voices> voices;
};

}  // namespace erachnid::starry
