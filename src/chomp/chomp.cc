#include "chomp.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

// #include "bridge.hh"
#include "gui/gui.hh"
#include "params.hh"
#include "reaper_plugin.h"

namespace erachnid::chomp {

const clap_plugin_descriptor_t plugin_desc = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "me.faec.erachnid.chomp",
    .name = "erachnid chomp",
    .vendor = "cubicvoid",
    .url = "https://faec.me",
    .manual_url = "https://your-domain.com/your-plugin/manual",
    .support_url = "https://your-domain.com/support",
    .version = "0.0.1",
    .description = "midi-triggered amp envelopes",
    .features = (const char *[]
    ){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

FILE *logFile = nullptr;

Plugin::Plugin(const clap_host_t *_host) : CLAPPlugin(_host, &plugin_desc) {
  static int count = 0;
  pluginID = count++;

  if (logFile == nullptr) {
    logFile = fopen("/Users/fae/chomp.log", "wb");
  }
  Log("plugin_create()");

  // init_plugin(&plugin, reinterpret_cast<void *>(this));
  // TODO: reactivate the gui
  // gui.reset(GUIWrapper::New(this));

  AddParam(
      ParamIDRats, "rats", "something", 0, 100, 50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  );
  AddParam(
      ParamIDAttack, "attack", "something else", 0.0, 1.0, 0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  );
  RefreshParameters();

  gui.reset(ChompGUI::New(this));
}

void Plugin::Log(const char *format...) {
  if (logFile != nullptr) {
    va_list args;
    va_start(args, format);

    char timeStr[64];
    int  msec;

    struct timeval tv;
    // time_t         t = static_cast<time_t>(tv.tv_sec);
    struct tm *timeinfo;
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

  const reaper_plugin_info_t *reaper =
      reinterpret_cast<const reaper_plugin_info_t *>(
          host->get_extension(host, "cockos.reaper_extension")
      );
  if (reaper != nullptr) {
    Log("Got a REAPER extension?!?!");
  }
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

}  // namespace erachnid::chomp
