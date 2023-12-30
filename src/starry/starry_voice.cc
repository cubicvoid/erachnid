#include "starry_voice.hh"

#include <clap/clap.h>
#include <math.h>

namespace erachnid::starry {

void StarryVoice::step() {
  // double phase = static_cast<double>(2.0 * M_PI * sample_pos) / 100.0;
  double radians = 2.0 * M_PI * static_cast<double>(phase) /
                   static_cast<double>(CLAP_SECTIME_FACTOR);

  L = 0.1 * cos(radians);
  R = 0.1 * sin(radians);

  sample_pos++;
  phase += freq;
}

bool StarryVoice::matches(const clap_event_note_t *event) {
  const bool chan_matches = (event->channel == -1 || channel == event->channel);
  const bool key_matches = (event->key == -1 || key == event->key);
  const bool note_id_matches = (note_id != -1 && note_id == event->note_id);
  return note_id_matches || (chan_matches && key_matches);
}

}  // namespace erachnid::starry