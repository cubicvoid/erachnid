#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

extern "C" const clap_plugin_descriptor_t chomp_plug_desc = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "me.faec.erachnid.bitesu",
    .name = "bites u",
    .vendor = "cubicvoid",
    .url = "https://faec.me",
    .manual_url = "https://your-domain.com/your-plugin/manual",
    .support_url = "https://your-domain.com/support",
    .version = "0.0.1",
    .description = "~fae~",
    .features = (const char *[]
    ){CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

void CLAP_ABI
nilLog(const clap_host_t *host, clap_log_severity severity, const char *msg) {}

static bool chomp_init(const struct clap_plugin *plugin) {
  ChompPlugin *plug = static_cast<ChompPlugin *>(plugin->plugin_data);
  plug->logFile = fopen("/Users/fae/chomp.log", "wb");

  // Fetch host's extensions here
  // Make sure to check that the interface functions are not null pointers
  const clap_host_log_t *ext_log = static_cast<const clap_host_log_t *>(
      plug->host->get_extension(plug->host, CLAP_EXT_LOG)
  );
  if(ext_log != nullptr && ext_log->log != nullptr) {
    plug->log = ext_log->log;
  }
  plug->host_thread_check =
      (const clap_host_thread_check_t *)plug->host->get_extension(
          plug->host, CLAP_EXT_THREAD_CHECK
      );
  plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(
      plug->host, CLAP_EXT_LATENCY
  );
  plug->host_state = (const clap_host_state_t *)plug->host->get_extension(
      plug->host, CLAP_EXT_STATE
  );
  return true;
}

static void chomp_destroy(const struct clap_plugin *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, "chomp_plug_destroy");
  if(plugin->logFile != nullptr) {
    fclose(plugin->logFile);
  }
  delete plugin;
}

static bool chomp_activate(
    const struct clap_plugin *_plugin,
    double                    sample_rate,
    uint32_t                  min_frames_count,
    uint32_t                  max_frames_count
) {
  char buf[256];
  snprintf(
      buf, sizeof(buf), "chomp_plug_activate(%lf, %d, %d)", sample_rate,
      min_frames_count, max_frames_count
  );
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  // plugin->log(plugin->host, CLAP_LOG_FATAL, "testing host log (the plugin
  // just activated)");
  return true;
}

static void chomp_deactivate(const struct clap_plugin *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, "chomp_plug_deactivate");
}

static bool chomp_start_processing(const struct clap_plugin *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->processing = true;
  return true;
}

static void chomp_stop_processing(const struct clap_plugin *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->processing = false;
}

static void chomp_reset(const struct clap_plugin *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
}

static const void *chomp_get_extension(
    const struct clap_plugin *_plugin, const char *id
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_get_extension(%s)", id);
  ChompPlugin *plugin = static_cast<ChompPlugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);

  // if (!strcmp(id, CLAP_EXT_LATENCY))
  //    return &s_my_plug_latency;
  if(strcmp(id, CLAP_EXT_AUDIO_PORTS) == 0) {
    return &s_chomp_audio_ports;
  }
  // if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
  //   return &s_chomp_note_ports;
  /*if (!strcmp(id, CLAP_EXT_STATE))
     return &s_my_plug_state;*/
  if(strcmp(id, CLAP_EXT_PARAMS) == 0) {
    return &s_chomp_params;
  }
  // TODO: add support to CLAP_EXT_PARAMS
  return NULL;
}

static void chomp_on_main_thread(const struct clap_plugin *plugin) {}

extern "C" clap_plugin_t *chomp_plug_create(const clap_host_t *host) {
  ChompPlugin *p = new ChompPlugin(host);
  return &p->plugin;
}

ChompPlugin::ChompPlugin(const clap_host_t *_host)
    : host(_host), log(nilLog), processing(false), active(false) {
  plugin.desc = &chomp_plug_desc;
  plugin.init = chomp_init;
  plugin.destroy = chomp_destroy;
  plugin.activate = chomp_activate;
  plugin.deactivate = chomp_deactivate;
  plugin.start_processing = chomp_start_processing;
  plugin.stop_processing = chomp_stop_processing;
  plugin.reset = chomp_reset;
  plugin.process = chomp_process;
  plugin.get_extension = chomp_get_extension;
  plugin.on_main_thread = chomp_on_main_thread;
  plugin.plugin_data = this;
}