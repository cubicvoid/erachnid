#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "clap_plugin.hh"
#include "params.hh"

namespace erachnid::chomp {

struct Voice {};

// Abstract class for Chomp (concrete implementations for each GUI
// platform, which so far is just darwin)
class ChompPlugin : public CLAPPlugin {
 public:
  ChompPlugin(const clap_host_t *_host);

  clap_process_status Process(const clap_process_t *process);

  bool Init();
  void Destroy();
  void Deactivate();
  bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );
  bool StartProcessing();
  void StopProcessing();
  void Reset();

  uint32_t NotePortsCount(bool is_input);
  bool NotePortsGet(uint32_t index, bool is_input, clap_note_port_info_t *info);

  uint32_t AudioPortsCount(bool is_input);
  bool     AudioPortsGet(
          uint32_t index, bool is_input, clap_audio_port_info_t *info
      );

  // FILE *logFile;

  void Log(const char *format...);

  void (*log)(const clap_host_t *host, int pluginID, const char *msg);

  uint32_t latency;

  int pluginID;

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
