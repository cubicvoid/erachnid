#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bridge.hh"
#include "chomp_impl.hh"

void CLAP_ABI
nilLog(const clap_host_t *host, clap_log_severity severity, const char *msg) {}

namespace chomp {

Plugin::Plugin(const clap_host_t *_host)
    : host(_host), log(nilLog), processing(false), active(false) {
  init_plugin(&plugin, reinterpret_cast<void *>(this));
}

bool Plugin::Init() {
  logFile = fopen("/Users/fae/chomp.log", "wb");

  // Fetch host's extensions here
  // Make sure to check that the interface functions are not null pointers
  const clap_host_log_t *ext_log = reinterpret_cast<const clap_host_log_t *>(
      host->get_extension(host, CLAP_EXT_LOG)
  );
  if (ext_log != nullptr && ext_log->log != nullptr) {
    log = ext_log->log;
  }
  host_thread_check = (const clap_host_thread_check_t *)host->get_extension(
      host, CLAP_EXT_THREAD_CHECK
  );
  host_latency =
      (const clap_host_latency_t *)host->get_extension(host, CLAP_EXT_LATENCY);
  host_state =
      (const clap_host_state_t *)host->get_extension(host, CLAP_EXT_STATE);
  return true;
}

void Plugin::Destroy() {
  flog(host, CLAP_LOG_INFO, "chomp_plug_destroy");
  if (logFile != nullptr) {
    fclose(logFile);
  }
}

bool Plugin::Activate(
    double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  char buf[256];
  snprintf(
      buf, sizeof(buf), "chomp_plug_activate(%lf, %d, %d)", sample_rate,
      min_frames_count, max_frames_count
  );
  flog(host, CLAP_LOG_INFO, buf);
  return true;
}

bool Plugin::Activate(
    double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  char buf[256];
  snprintf(
      buf, sizeof(buf), "chomp_plug_activate(%lf, %d, %d)", sample_rate,
      min_frames_count, max_frames_count
  );
  flog(host, CLAP_LOG_INFO, buf);
  // plugin->log(plugin->host, CLAP_LOG_FATAL, "testing host log (the plugin
  // just activated)");
  return true;
}

void Plugin::Deactivate() {
  char buf[128];
  snprintf(
      buf, sizeof(buf),
      "chomp_plug_deactivate (%d note on, %d note off, %d midi)", noteOnCount,
      noteOffCount, midiCount
  );
  flog(nullptr, CLAP_LOG_INFO, buf);
}

void Plugin::Reset() {}

}  // namespace chomp
