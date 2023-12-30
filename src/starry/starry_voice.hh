#pragma once

#include <clap/clap.h>

namespace erachnid::starry {

struct StarryVoice {
  StarryVoice *next;
  enum AEGMode { OFF, ATTACK, HOLD, NEWLY_OFF, RELEASING } state{OFF};

  int portid;   // clap note port index
  int channel;  // midi channel
  int key;      // The midi key which triggered me
  int note_id;  // and the note_id delivered by the host (used for note
                // expressions)

  int first_sample;
  // Finally, please set my sample rate at voice on. Thanks!
  float sample_rate{0};

  // current position in cycle, between 0 (inclusive) and 1 (exclusive),
  // using 33.31 fixed point (see clap/fixedpoint.h)
  int64_t phase;

  // current frequency, in fixed-point cycles per sample. added to phase every
  // sample.
  int64_t freq;

  int sample_pos{0};

  float L{0.f}, R{0.f};

  bool isPlaying() { return (state == ATTACK); }
  void release() { state = NEWLY_OFF; }
  void step();
  bool matches(const clap_event_note_t *event);
};

}  // namespace erachnid::starry