#pragma once

namespace erachnid::starry {

struct StarryVoice {
  enum AEGMode { OFF, ATTACK, HOLD, NEWLY_OFF, RELEASING } state{OFF};

  int portid;   // clap note port index
  int channel;  // midi channel
  int key;      // The midi key which triggered me
  int note_id;  // and the note_id delivered by the host (used for note
                // expressions)

  int first_sample;
  // Finally, please set my sample rate at voice on. Thanks!
  float sample_rate{0};

  int sample_pos{0};

  float L{0.f}, R{0.f};

  bool isPlaying() { return (state == ATTACK); }
  void release() { state = NEWLY_OFF; }
  void step();
};

}  // namespace erachnid::starry