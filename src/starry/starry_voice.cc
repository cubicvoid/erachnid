#include "starry_voice.hh"

#include <math.h>

namespace erachnid::starry {

void StarryVoice::step() {
  double phase = static_cast<double>(sample_pos) / 100.0;
  L = 0.1 * cos(phase);
  R = 0.1 * sin(phase);

  sample_pos++;
}

}  // namespace erachnid::starry