#pragma once

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>

#include "clap_param.hh"
#include "clap_plugin.hh"

#include <nlohmann/json.hpp>


namespace erachnid::scan {

struct SerializedEvent {
  uint32_t time;
  uint16_t space_id;
  uint16_t type;
};

struct SerializedProcessCall {
  uint32_t frames_count;
  std::vector<SerializedEvent> events;
};
  
  
class Plugin : public CLAPPlugin {
 public:
  Plugin(const clap_host_t *_host);

  virtual void OnMainThread();
  virtual clap_process_status Process(const clap_process_t *process);
  virtual void ParamsFlush(
    const clap_input_events_t  *in, const clap_output_events_t *out);

  virtual uint32_t NotePortsCount(bool is_input) { return 1; }
  virtual uint32_t AudioPortsCount(bool is_input) { return 1; }

  uint32_t latency;
  
  nlohmann::json GetData();
  void ResetLog();

protected:
  virtual void setEventCount(int count) { };

 private:
  void NoteOn(const clap_event_note_t *note);
  void NoteOff(const clap_event_note_t *note);

  // entries should only be accessed on the main thread. pending_entries
  // should only be accessed while holding pending_entries_lock.
  std::vector<nlohmann::json> entries;
  std::vector<nlohmann::json> pending_entries;
  std::mutex pending_entries_lock;
};

}  // namespace erachnid::skeleton
