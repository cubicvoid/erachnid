#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

#include "bridge.hh"
#include "chomp_impl.hh"
#include "gui/gui.h"
#include "params.hh"
#include "reaper_plugin.h"

void CLAP_ABI nilLog(const clap_host_t *host, int pluginID, const char *msg) {}

namespace chomp {

FILE *logFile = nullptr;

Plugin::Plugin(const clap_host_t *_host)
    : host(_host), log(nilLog), processing(false), active(false) {
  static int count = 0;
  pluginID = count++;

  if (logFile == nullptr) {
    logFile = fopen("/Users/fae/chomp.log", "wb");
  }
  Log("plugin_create()");

  init_plugin(&plugin, reinterpret_cast<void *>(this));
  gui.reset(GUIWrapper::New(this));

  AddParam(
      ParamIDRats, "rats", "something", 0, 100, 50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  );
  AddParam(
      ParamIDAttack, "attack", "something else", 0.0, 1.0, 0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  );
  RefreshParameters();
}

void Plugin::Log(const char *format...) {
  if (logFile != nullptr) {
    va_list args;
    va_start(args, format);

    char timeStr[64];
    int  msec;

    struct timeval tv;
    time_t         t = static_cast<time_t>(tv.tv_sec);
    struct tm     *timeinfo;
    gettimeofday(&tv, nullptr);
    timeinfo = localtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%F %T", timeinfo);
    msec = static_cast<int>(tv.tv_usec / 1000);

    char buf[256];
    snprintf(
        buf, sizeof(buf), "[%d] %s.%03d %s\n", pluginID, timeStr, msec, format
    );
    vfprintf(logFile, buf, args);
    fflush(logFile);
  }
}

bool Plugin::Init() {
  Log("plugin_init");

  // Fetch host's extensions here
  // Make sure to check that the interface functions are not null pointers
  const clap_host_log_t *ext_log = reinterpret_cast<const clap_host_log_t *>(
      host->get_extension(host, CLAP_EXT_LOG)
  );
  if (ext_log != nullptr && ext_log->log != nullptr) {
    log = ext_log->log;
  }

  const clap_host_gui_t *ext_gui = reinterpret_cast<const clap_host_gui_t *>(
      host->get_extension(host, CLAP_EXT_GUI)
  );
  if (ext_gui != nullptr) {
    // ext_gui->request_show()
    // ext_gui->
  }

  const reaper_plugin_info_t *reaper =
      reinterpret_cast<const reaper_plugin_info_t *>(
          host->get_extension(host, "cockos.reaper_extension")
      );
  if (reaper != nullptr) {
    Log("Got a REAPER extension?!?!");
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
  Log("chomp_plug_destroy");
  /*if (logFile != nullptr) {
    fclose(logFile);
  }*/
}

bool Plugin::Activate(
    double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count
) {
  Log("chomp_plug_activate(%lf, %d, %d)", sample_rate, min_frames_count,
      max_frames_count);
  return true;
}

void Plugin::Deactivate() {
  Log("chomp_plug_deactivate (%d note on, %d note off, %d midi, %d samples)",
      noteOnCount, noteOffCount, midiCount, sampleCount);
}

void Plugin::Reset() {}

}  // namespace chomp
