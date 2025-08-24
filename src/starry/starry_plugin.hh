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
#include "starry_voice.hh"

namespace erachnid::starry {

class StarryPlugin : public CLAPPlugin {
 public:
  static constexpr int max_voices = 8;

  StarryPlugin(const clap_host_t *_host);
  virtual ~StarryPlugin() { }

  virtual bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );

  virtual uint32_t NotePortsCount(bool is_input) { return is_input ? 1 : 0; };
  virtual uint32_t AudioPortsCount(bool is_input) {
    return is_input ? 0 : 1;
  };

  virtual clap_process_status Process(const clap_process_t *process);

  void ProcessEvent(const clap_event_header_t *hdr);

 private:
  void handleNoteOn(const clap_event_note_t *event);
  void handleNoteOff(const clap_event_note_t *event);
  void handleNoteChoke(const clap_event_note_t *event);

  StarryVoice *chooseNewVoice();
  void         activateVoice(StarryVoice *v, const clap_event_note_t *event);
  void         chokeVoice(StarryVoice *v);

  std::array<StarryVoice, max_voices>         voices;
  std::vector<std::tuple<int, int, int, int>> terminated_voices;

  int sample_pos{0};
};

}  // namespace erachnid::starry
