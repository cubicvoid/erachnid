// Interface between CLAP's C API and the C++ plugin object

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"
#include "gui/gui.h"

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
    ){CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

}  // namespace chomp

namespace {

using namespace chomp;

bool chomp_init(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->Init();
}

void chomp_destroy(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Destroy();
  delete plugin;
}

bool chomp_activate(
    const struct clap_plugin *_plugin,
    double                    sample_rate,
    uint32_t                  min_frames_count,
    uint32_t                  max_frames_count
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->Activate(sample_rate, min_frames_count, max_frames_count);
}

void chomp_deactivate(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Deactivate();
}

bool chomp_start_processing(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->StartProcessing();
}

void chomp_stop_processing(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->StopProcessing();
}

clap_process_status chomp_process(
    const struct clap_plugin *_plugin, const clap_process_t *process
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->Process(process);
}

void chomp_reset(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Reset();
}

/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////

uint32_t chomp_audio_ports_count(const clap_plugin_t *_plugin, bool is_input) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->AudioPortsCount(is_input);
}

bool chomp_audio_ports_get(
    const clap_plugin_t    *_plugin,
    uint32_t                index,
    bool                    is_input,
    clap_audio_port_info_t *info
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->AudioPortsGet(index, is_input, info);
}

const clap_plugin_audio_ports_t s_chomp_audio_ports = {
    .count = chomp_audio_ports_count,
    .get = chomp_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

uint32_t chomp_note_ports_count(const clap_plugin_t *_plugin, bool is_input) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->NotePortsCount(is_input);
}

bool chomp_note_ports_get(
    const clap_plugin_t   *_plugin,
    uint32_t               index,
    bool                   is_input,
    clap_note_port_info_t *info
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->NotePortsGet(index, is_input, info);
}

const clap_plugin_note_ports_t s_chomp_note_ports = {
    .count = chomp_note_ports_count,
    .get = chomp_note_ports_get,
};

////////////////////////
// clap_plugin_params //
////////////////////////

uint32_t chomp_params_count(const clap_plugin_t *_plugin) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  return plugin->ParamCount();
}

bool chomp_params_get_info(
    const clap_plugin_t *_plugin,
    uint32_t             param_index,
    clap_param_info_t   *param_info
) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  plugin->Log("params_get_info(%d)", param_index);
  return plugin->ParamGetInfo(param_index, param_info);
}

bool chomp_params_get_value(
    const clap_plugin_t *_plugin, clap_id param_id, double *out_value
) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  return plugin->ParamGetValue(param_id, out_value);
}

bool chomp_params_value_to_text(
    const clap_plugin_t *_plugin,
    clap_id              param_id,
    double               value,
    char                *out_buffer,
    uint32_t             out_buffer_capacity
) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  return plugin->ParamValueToText(
      param_id, value, out_buffer, out_buffer_capacity
  );
}

bool chomp_params_text_to_value(
    const clap_plugin_t *_plugin,
    clap_id              param_id,
    const char          *param_value_text,
    double              *out_value
) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  return plugin->ParamTextToValue(param_id, param_value_text, out_value);
}

void chomp_params_flush(
    const clap_plugin_t        *_plugin,
    const clap_input_events_t  *in,
    const clap_output_events_t *out
) {
  Plugin *plugin = reinterpret_cast<Plugin *>(_plugin->plugin_data);
  plugin->ParamFlush(in, out);
}

const clap_plugin_params_t s_chomp_params = {
    .count = chomp_params_count,
    .get_info = chomp_params_get_info,
    .get_value = chomp_params_get_value,
    .value_to_text = chomp_params_value_to_text,
    .text_to_value = chomp_params_text_to_value,
    .flush = chomp_params_flush,
};

/////////////////////
// clap_plugin_gui //
/////////////////////

bool chomp_gui_is_api_supported(
    const clap_plugin_t *_plugin, const char *api, bool is_floating
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Log("is_api_supported(%s, %d)", api, is_floating);
  return strcmp(api, CLAP_WINDOW_API_COCOA) == 0 && !is_floating;
}

bool chomp_gui_get_preferred_api(
    const clap_plugin_t *_plugin, const char **api, bool *is_floating
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Log("get_preferred_api()");
  *api = CLAP_WINDOW_API_COCOA;
  *is_floating = false;
  return true;
}

bool chomp_gui_create(
    const clap_plugin_t *_plugin, const char *api, bool is_floating
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->Create(api, is_floating);
}

void chomp_gui_destroy(const clap_plugin_t *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->Destroy();
}

bool chomp_gui_set_scale(const clap_plugin_t *_plugin, double scale) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->SetScale(scale);
}

bool chomp_gui_get_size(
    const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->GetSize(width, height);
}

bool chomp_gui_can_resize(const clap_plugin_t *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->CanResize();
  return true;
}

bool chomp_gui_get_resize_hints(
    const clap_plugin_t *_plugin, clap_gui_resize_hints_t *hints
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Log("gui_get_resize_hints()");

  // TODO
  return false;
}

bool chomp_gui_adjust_size(
    const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->AdjustSize(width, height);
}

bool chomp_gui_set_size(
    const clap_plugin_t *_plugin, uint32_t width, uint32_t height
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->SetSize(width, height);
}

bool chomp_gui_set_parent(
    const clap_plugin_t *_plugin, const clap_window_t *window
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->SetParent(window);
}

bool chomp_gui_set_transient(
    const clap_plugin_t *_plugin, const clap_window_t *window
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->SetTransient(window);
}

void chomp_gui_suggest_title(const clap_plugin_t *plugin, const char *title) {
  // TODO: low priority since this is floating only
}

bool chomp_gui_show(const clap_plugin_t *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->Show();
}

bool chomp_gui_hide(const clap_plugin_t *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  return plugin->gui->Hide();
}

const clap_plugin_gui_t s_chomp_gui = {
    .is_api_supported = chomp_gui_is_api_supported,
    .get_preferred_api = chomp_gui_get_preferred_api,
    .create = chomp_gui_create,
    .destroy = chomp_gui_destroy,
    .set_scale = chomp_gui_set_scale,
    .get_size = chomp_gui_get_size,
    .can_resize = chomp_gui_can_resize,
    .get_resize_hints = chomp_gui_get_resize_hints,
    .adjust_size = chomp_gui_adjust_size,
    .set_size = chomp_gui_set_size,
    .set_parent = chomp_gui_set_parent,
    .set_transient = chomp_gui_set_transient,
    .suggest_title = chomp_gui_suggest_title,
    .show = chomp_gui_show,
    .hide = chomp_gui_hide,
};

//
// extension handling
//

const void *chomp_get_extension(
    const struct clap_plugin *_plugin, const char *id
) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Log("chomp_get_extension(%s)", id);

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

  if (strcmp(id, CLAP_EXT_GUI) == 0) {
    return &s_chomp_gui;
  }
  // TODO: add support to CLAP_EXT_PARAMS
  return NULL;
}

void chomp_on_main_thread(const struct clap_plugin *_plugin) {
  chomp::Plugin *plugin =
      reinterpret_cast<chomp::Plugin *>(_plugin->plugin_data);
  plugin->Log("chomp_on_main_thread()");
  /*const clap_host_params_t *host_params =
      reinterpret_cast<const clap_host_params_t *>(
          plugin->host->get_extension(plugin->host, CLAP_EXT_PARAMS)
      );
  if (host_params != nullptr && host_params->rescan != nullptr) {
    host_params->rescan(plugin->host, CLAP_PARAM_RESCAN_ALL);
  }*/
}

}  // namespace

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
