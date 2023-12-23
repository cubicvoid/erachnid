#pragma once

struct StarryVoice {
  static constexpr int max_uni = 7;

  int portid;   // clap note port index
  int channel;  // midi channel
  int key;      // The midi key which triggered me
  int note_id;  // and the note_id delivered by the host (used for note
                // expressions)

  // unison count is snapped at voice on
  int unison{3};

  // Note the pattern that we have an item and its modulator as the API
  float uniSpread{10.0}, uniSpreadMod{0.0};

  // The oscillator detuning
  float oscDetune{0}, oscDetuneMod{0};

  // Filter characteristics. After adjusting these call 'recalcFilter'.
  int   filterMode{StereoSimperSVF::Mode::LP};
  float cutoff{69.0}, res{0.7};
  float cutoffMod{0.0}, resMod{0.0};

  // The internal AEG is incredibly simple. Bypass or not, and have
  // an attack and release time in seconds. These aren't modulatable
  // mostly out of laziness.
  bool  ampGate{false};
  float ampAttack{0.01}, ampRelease{0.1};

  // The pre-filter VCA is unique in that it can be either internally
  // modulated and externally modulated. If ampGate is false, the internal
  // modulation is bypassed. The two vectors for modulation are a VCAMod
  // value, intended for param modulation, and a volumeNoteExpressionValue
  float preFilterVCA{1.0}, preFilterVCAMod{0.0}, volumeNoteExpressionValue{0.f};

  // Two values can modify pitch, the note expression and the bend wheel.
  // After adjusting these, call 'recalcPitch'
  float pitchNoteExpressionValue{0.f}, pitchBendWheel{0.f};

  // Finally, please set my sample rate at voice on. Thanks!
  float sampleRate{0};
};
