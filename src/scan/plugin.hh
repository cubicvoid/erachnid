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

class Plugin : public CLAPPlugin {
 public:
  Plugin(const clap_host_t *_host);

  virtual clap_process_status Process(const clap_process_t *process);
  virtual void ParamsFlush(
    const clap_input_events_t  *in, const clap_output_events_t *out);

  virtual uint32_t NotePortsCount(bool is_input) { return 1; }
  virtual uint32_t AudioPortsCount(bool is_input) { return 1; }

  virtual bool Activate(
      double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
  );
  virtual void Deactivate();
  virtual bool StartProcessing();
  virtual void StopProcessing();
  virtual void Reset();
  virtual void OnMainThread();

  virtual bool StateSaveToJSON(nlohmann::json json);
  virtual bool StateLoadFromJSON(nlohmann::json json);

  uint32_t latency;
  
  nlohmann::json GetData();
  void ResetLog();

  void setIncludeEmptyProcess(bool value) {
		include_empty_process.store(value);
    MarkPluginStateDirty();
	}

  bool getIncludeEmptyProcess() {
    return include_empty_process.load();
  }

  void setIncludeOnMainThread(bool value) {
    include_on_main_thread = value;
    MarkPluginStateDirty();
  }

  bool getIncludeOnMainThread() {
    return include_on_main_thread;
  }

  void setIncludeProcessWithoutTransport(bool value) {
    include_no_transport.store(value);
    MarkPluginStateDirty();
  }

  bool getIncludeProcessWithoutTransport() {
    return include_no_transport.load();
  }

protected:
  virtual void setEventCount(int count) { };

 private:

  void AddEntryFromMainThread(nlohmann::json entry);
  void AddEntryFromAudioThread(nlohmann::json entry);

  void handleEvent(const clap_event_header_t *hdr);


  void refreshEntries();
  // entries should only be accessed on the main thread. pending_entries
  // should only be accessed while holding pending_entries_lock.
  std::vector<nlohmann::json> entries;
  std::vector<nlohmann::json> pending_entries;
  std::mutex pending_entries_lock;
  
  std::atomic<bool> include_empty_process;
  std::atomic<bool> include_no_transport;
  bool include_on_main_thread;
  std::atomic<int64_t> steady_time_calculated;

  CLAPParam *_param_rats;
  CLAPParam *_param_attack;
  CLAPParam *_param_decibels;
};

}  // namespace erachnid::skeleton
