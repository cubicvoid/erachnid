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
  float sampleRate{0};

  bool isPlaying() { return (state == ATTACK); }
  void release() { state = NEWLY_OFF; }
};

}  // namespace erachnid::starry