// Interface between CLAP's C API and the C++ plugin object

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

namespace chomp {

clap_plugin_descriptor_t plugin_desc = {
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

}  // namespace chomp

static bool chomp_init(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->Init();
}

static void chomp_destroy(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Destroy();
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
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  // plugin->log(plugin->host, CLAP_LOG_FATAL, "testing host log (the plugin
  // just activated)");
  return true;
}

static void chomp_deactivate(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Deactivate();
}

static bool chomp_start_processing(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->StartProcessing();
  plugin->processing = true;
  return true;
}

static void chomp_stop_processing(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->StopProcessing();
  plugin->processing = false;
}

static clap_process_status chomp_process(
    const struct clap_plugin *_plugin, const clap_process_t *process
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->Process(process);
}

static void chomp_reset(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Reset();
}

/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////

static uint32_t chomp_audio_ports_count(
    const clap_plugin_t *_plugin, bool is_input
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_audio_ports_count(is_input: %d)", is_input);
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  // We just declare 1 audio input and 1 audio output
  return 1;
}

static bool chomp_audio_ports_get(
    const clap_plugin_t    *_plugin,
    uint32_t                index,
    bool                    is_input,
    clap_audio_port_info_t *info
) {
  char buf[256];
  snprintf(
      buf, sizeof(buf), "chomp_audio_ports_get(index: %d, is_input: %d)", index,
      is_input
  );
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
  if (index > 0) {
    return false;
  }
  info->id = 0;
  if (is_input) {
    snprintf(info->name, sizeof(info->name), "Input");
  } else {
    snprintf(info->name, sizeof(info->name), "Output");
  }
  info->channel_count = 2;
  info->flags = CLAP_AUDIO_PORT_IS_MAIN | CLAP_AUDIO_PORT_SUPPORTS_64BITS;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}

const clap_plugin_audio_ports_t s_chomp_audio_ports = {
    .count = chomp_audio_ports_count,
    .get = chomp_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t chomp_note_ports_count(
    const clap_plugin_t *_plugin, bool is_input
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->flog(nullptr, 0, "chomp_note_ports_count");
  return 1;
}

static bool chomp_note_ports_get(
    const clap_plugin_t   *plugin,
    uint32_t               index,
    bool                   is_input,
    clap_note_port_info_t *info
) {
  if (index > 0) return false;
  info->id = 0;
  snprintf(info->name, sizeof(info->name), "Trigger");
  info->supported_dialects = CLAP_NOTE_DIALECT_CLAP;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  return true;
}

const clap_plugin_note_ports_t s_chomp_note_ports = {
    .count = chomp_note_ports_count,
    .get = chomp_note_ports_get,
};

static uint32_t chomp_params_count(const clap_plugin_t *_plugin) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->ParamsCount();
}

static bool chomp_params_get_info(
    const clap_plugin_t *_plugin,
    uint32_t             param_index,
    clap_param_info_t   *param_info
) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->ParamsGetInfo(param_index, param_info);
}

bool chomp_params_get_value(
    const clap_plugin_t *_plugin, clap_id param_id, double *out_value
) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->ParamsGetValue(param_id, out_value);
}

bool chomp_params_value_to_text(
    const clap_plugin_t *_plugin,
    clap_id              param_id,
    double               value,
    char                *out_buffer,
    uint32_t             out_buffer_capacity
) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->ParamsValueToText(
      param_id, value, out_buffer, out_buffer_capacity
  );
}

bool chomp_params_text_to_value(
    const clap_plugin_t *_plugin,
    clap_id              param_id,
    const char          *param_value_text,
    double              *out_value
) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->ParamsTextToValue(param_id, param_value_text, out_value);
}

void chomp_params_flush(
    const clap_plugin_t        *_plugin,
    const clap_input_events_t  *in,
    const clap_output_events_t *out
) {
  chomp::Plugin *plugin = static_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->ParamsFlush(in, out);
}

const clap_plugin_params_t s_chomp_params = {
    .count = chomp_params_count,
    .get_info = chomp_params_get_info,
    .get_value = chomp_params_get_value,
    .value_to_text = chomp_params_value_to_text,
    .text_to_value = chomp_params_text_to_value,
    .flush = chomp_params_flush,
};

static const void *chomp_get_extension(
    const struct clap_plugin *_plugin, const char *id
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_get_extension(%s)", id);
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);

  // if (!strcmp(id, CLAP_EXT_LATENCY))
  //    return &s_my_plug_latency;
  if (strcmp(id, CLAP_EXT_AUDIO_PORTS) == 0) {
    return &s_chomp_audio_ports;
  } else if (strcmp(id, CLAP_EXT_NOTE_PORTS) == 0) {
    return &s_chomp_note_ports;
  }
  /*if (!strcmp(id, CLAP_EXT_STATE))
     return &s_my_plug_state;*/
  if (strcmp(id, CLAP_EXT_PARAMS) == 0) {
    return &s_chomp_params;
  }
  // TODO: add support to CLAP_EXT_PARAMS
  return NULL;
}

static void chomp_on_main_thread(const struct clap_plugin *plugin) {}

namespace chomp {

clap_plugin_t *plugin_create(const clap_host_t *host) {
  chomp::Plugin *p = new chomp::Plugin(host);
  return &p->plugin;
}

void init_plugin(clap_plugin_t *plugin, void *data) {
  plugin->desc = &plugin_desc;
  plugin->init = chomp_init;
  plugin->destroy = chomp_destroy;
  plugin->activate = chomp_activate;
  plugin->deactivate = chomp_deactivate;
  plugin->start_processing = chomp_start_processing;
  plugin->stop_processing = chomp_stop_processing;
  plugin->reset = chomp_reset;
  plugin->process = chomp_process;
  plugin->get_extension = chomp_get_extension;
  plugin->on_main_thread = chomp_on_main_thread;
  plugin->plugin_data = data;
}

}  // namespace chomp
