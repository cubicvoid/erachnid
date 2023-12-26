#include "starry_voice.hh"

#include <clap/fixedpoint.h>
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

}  // namespace erachnid::starry