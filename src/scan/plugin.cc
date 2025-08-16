#include "plugin.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>



#ifdef DARWIN
#include "gui/gui_darwin.h"
#endif

// #include "bridge.hh"
// #include "gui/gui.hh"
#include "clap_param.hh"
#include "reaper_plugin.h"

enum { PARAM_RATS, PARAM_ATTACK };

namespace erachnid::scan {

  using nlohmann::json;

extern const clap_plugin_descriptor_t plugin_desc;

Plugin::Plugin(const clap_host_t *_host) : CLAPPlugin(_host, &plugin_desc) {
  Log("plugin_create()");

  AddParam(new CLAPParam(
      PARAM_RATS, "rats", "something", 0, 100, 50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  ));
  AddParam(new CLAPParam(
      PARAM_ATTACK, "attack", "something else", 0.0, 1.0, 0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  ));
}

bool Plugin::Activate(
  double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  json j;
  j["method"] = std::string("activate");
  j["sample_rate"] = sample_rate;
  j["min_frames_count"] = min_frames_count;
  j["max_frames_count"] = max_frames_count;
  AddEntryFromMainThread(j);

  return CLAPPlugin::Activate(sample_rate, min_frames_count, max_frames_count);
}

void Plugin::AddEntryFromMainThread(json entry) {
  entry["steady_time_calculated"] = steady_time_calculated.load();
  refreshEntries();
  entries.push_back(entry);
}

void Plugin::AddEntryFromAudioThread(json entry) {
  entry["steady_time_calculated"] = steady_time_calculated.load();
  pending_entries_lock.lock();
  pending_entries.push_back(entry);
  pending_entries_lock.unlock();
  RequestMainThreadCallback();
}

void Plugin::Deactivate() {
  json j;
  j["method"] = std::string("deactivate");
  AddEntryFromMainThread(j);
  CLAPPlugin::Deactivate();
}

bool Plugin::StartProcessing() {
  json j;
  j["method"] = std::string("start_processing");
  AddEntryFromAudioThread(j);
  return CLAPPlugin::StartProcessing();
}

void Plugin::StopProcessing() {
  json j;
  j["method"] = std::string("stop_processing");
  AddEntryFromAudioThread(j);
  CLAPPlugin::StopProcessing();
}

void Plugin::Reset() {
  json j;
  j["method"] = std::string("reset");
  AddEntryFromAudioThread(j);
  CLAPPlugin::Reset();
}

void Plugin::OnMainThread() {
  if (include_on_main_thread) {
    json j;
    j["method"] = std::string("on_main_thread");
    AddEntryFromMainThread(j);
  } else {
    refreshEntries();
  }

  setEventCount(static_cast<int>(entries.size()));
}

// must be called on the main thread
void Plugin::refreshEntries() {
  std::vector<nlohmann::json> new_entries;
  pending_entries_lock.lock();
  std::swap(pending_entries, new_entries);
  pending_entries_lock.unlock();

  entries.insert(entries.end(), new_entries.begin(), new_entries.end());
}

nlohmann::json Plugin::GetData() {
	nlohmann::json result = entries;
	return result;
}


void Plugin::ResetLog() {
	refreshEntries();
  entries.clear();
  setEventCount(static_cast<int>(entries.size()));
}

}  // namespace erachnid::scan
