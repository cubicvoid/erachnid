// Interface between CLAP's C API and the C++ plugin object

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "clap_gui.hh"
#include "clap_plugin.hh"
// #include "reaper_plugin.h"
// #include "reaper_plugin_functions.h"

namespace erachnid {

namespace {

  CLAP_ABI bool plugin_init(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->Init();
  }

  CLAP_ABI void plugin_destroy(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->Destroy();
    delete plugin;
  }

  CLAP_ABI bool plugin_activate(
      const struct clap_plugin *_plugin,
      double                    sample_rate,
      uint32_t                  min_frames_count,
      uint32_t                  max_frames_count
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->Activate(sample_rate, min_frames_count, max_frames_count);
  }

  void plugin_deactivate(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->Deactivate();
  }

  bool plugin_start_processing(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->StartProcessing();
  }

  void plugin_stop_processing(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->StopProcessing();
  }

  clap_process_status plugin_process(
      const struct clap_plugin *_plugin, const clap_process_t *process
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->Process(process);
  }

  void plugin_reset(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->Reset();
  }

  /////////////////////////////
  // clap_plugin_audio_ports //
  /////////////////////////////

  uint32_t plugin_audio_ports_count(
      const clap_plugin_t *_plugin, bool is_input
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->AudioPortsCount(is_input);
  }

  bool plugin_audio_ports_get(
      const clap_plugin_t    *_plugin,
      uint32_t                index,
      bool                    is_input,
      clap_audio_port_info_t *info
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->AudioPortsGet(index, is_input, info);
  }

  const clap_plugin_audio_ports_t s_plugin_audio_ports = {
      .count = plugin_audio_ports_count,
      .get = plugin_audio_ports_get,
  };

  ////////////////////////////
  // clap_plugin_note_ports //
  ////////////////////////////

  uint32_t plugin_note_ports_count(
      const clap_plugin_t *_plugin, bool is_input
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->NotePortsCount(is_input);
  }

  bool plugin_note_ports_get(
      const clap_plugin_t   *_plugin,
      uint32_t               index,
      bool                   is_input,
      clap_note_port_info_t *info
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->NotePortsGet(index, is_input, info);
  }

  const clap_plugin_note_ports_t s_plugin_note_ports = {
      .count = plugin_note_ports_count,
      .get = plugin_note_ports_get,
  };

  ////////////////////////
  // clap_plugin_params //
  ////////////////////////

  uint32_t plugin_params_count(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamCount();
  }

  bool plugin_params_get_info(
      const clap_plugin_t *_plugin,
      uint32_t             param_index,
      clap_param_info_t   *param_info
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamGetInfo(param_index, param_info);
  }

  bool plugin_params_get_value(
      const clap_plugin_t *_plugin, clap_id param_id, double *out_value
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamGetValue(param_id, out_value);
  }

  bool plugin_params_value_to_text(
      const clap_plugin_t *_plugin,
      clap_id              param_id,
      double               value,
      char                *out_buffer,
      uint32_t             out_buffer_capacity
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamValueToText(
        param_id, value, out_buffer, out_buffer_capacity
    );
  }

  bool plugin_params_text_to_value(
      const clap_plugin_t *_plugin,
      clap_id              param_id,
      const char          *param_value_text,
      double              *out_value
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamTextToValue(param_id, param_value_text, out_value);
  }

  void plugin_params_flush(
      const clap_plugin_t        *_plugin,
      const clap_input_events_t  *in,
      const clap_output_events_t *out
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->ParamFlush(in, out);
  }

  const clap_plugin_params_t s_plugin_params = {
      .count = plugin_params_count,
      .get_info = plugin_params_get_info,
      .get_value = plugin_params_get_value,
      .value_to_text = plugin_params_value_to_text,
      .text_to_value = plugin_params_text_to_value,
      .flush = plugin_params_flush,
  };

  /////////////////////
  // clap_plugin_gui //
  /////////////////////

  bool plugin_gui_is_api_supported(
      const clap_plugin_t *_plugin, const char *api, bool is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIIsAPISupported(api, is_floating);
  }

  bool plugin_gui_get_preferred_api(
      const clap_plugin_t *_plugin, const char **api, bool *is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIGetPreferredAPI(api, is_floating);
  }

  bool plugin_gui_create(
      const clap_plugin_t *_plugin, const char *api, bool is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUICreate(api, is_floating);
  }

  void plugin_gui_destroy(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIDestroy();
  }

  bool plugin_gui_set_scale(const clap_plugin_t *_plugin, double scale) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetScale(scale);
  }

  bool plugin_gui_get_size(
      const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIGetSize(width, height);
  }

  bool plugin_gui_can_resize(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUICanResize();
  }

  bool plugin_gui_get_resize_hints(
      const clap_plugin_t *_plugin, clap_gui_resize_hints_t *hints
  ) {
    // CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin
    // *>(_plugin->plugin_data);

    // TODO
    return false;
  }

  bool plugin_gui_adjust_size(
      const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIAdjustSize(width, height);
  }

  bool plugin_gui_set_size(
      const clap_plugin_t *_plugin, uint32_t width, uint32_t height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetSize(width, height);
  }

  bool plugin_gui_set_parent(
      const clap_plugin_t *_plugin, const clap_window_t *window
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetParent(window);
  }

  bool plugin_gui_set_transient(
      const clap_plugin_t *_plugin, const clap_window_t *window
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetTransient(window);
  }

  void plugin_gui_suggest_title(
      const clap_plugin_t *plugin, const char *title
  ) {
    // TODO: low priority since this is floating only
  }

  bool plugin_gui_show(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIShow();
  }

  bool plugin_gui_hide(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIHide();
  }

  const clap_plugin_gui_t s_plugin_gui = {
      .is_api_supported = plugin_gui_is_api_supported,
      .get_preferred_api = plugin_gui_get_preferred_api,
      .create = plugin_gui_create,
      .destroy = plugin_gui_destroy,
      .set_scale = plugin_gui_set_scale,
      .get_size = plugin_gui_get_size,
      .can_resize = plugin_gui_can_resize,
      .get_resize_hints = plugin_gui_get_resize_hints,
      .adjust_size = plugin_gui_adjust_size,
      .set_size = plugin_gui_set_size,
      .set_parent = plugin_gui_set_parent,
      .set_transient = plugin_gui_set_transient,
      .suggest_title = plugin_gui_suggest_title,
      .show = plugin_gui_show,
      .hide = plugin_gui_hide,
  };

  //
  // extension handling
  //

  const void *plugin_get_extension(
      const struct clap_plugin *_plugin, const char *id
  ) {
    // CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin
    // *>(_plugin->plugin_data);

    // if (!strcmp(id, CLAP_EXT_LATENCY))
    //    return &s_my_plug_latency;
    if (strcmp(id, CLAP_EXT_AUDIO_PORTS) == 0) {
      return &s_plugin_audio_ports;
    } else if (strcmp(id, CLAP_EXT_NOTE_PORTS) == 0) {
      return &s_plugin_note_ports;
    }
    /*if (!strcmp(id, CLAP_EXT_STATE))
       return &s_my_plug_state;*/
    if (strcmp(id, CLAP_EXT_PARAMS) == 0) {
      return &s_plugin_params;
    }

    if (strcmp(id, CLAP_EXT_GUI) == 0) {
      return &s_plugin_gui;
    }
    // TODO: add support to CLAP_EXT_PARAMS
    return NULL;
  }

  void plugin_on_main_thread(const struct clap_plugin *_plugin) {
    // CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin
    // *>(_plugin->plugin_data);
    /*const clap_host_params_t *host_params =
        reinterpret_cast<const clap_host_params_t *>(
            plugin->host->get_extension(plugin->host, CLAP_EXT_PARAMS)
        );
    if (host_params != nullptr && host_params->rescan != nullptr) {
      host_params->rescan(plugin->host, CLAP_PARAM_RESCAN_ALL);
    }*/
  }

}  // namespace

/*clap_plugin_t *plugin_create(const clap_host_t *host) {
  chomp::Plugin *p = new chomp::Plugin(host);
  return &p->plugin;
}*/

void CLAPPlugin::InitRawPlugin(const clap_plugin_descriptor_t *desc) {
  rawPlugin.desc = desc;
  rawPlugin.init = plugin_init;
  rawPlugin.destroy = plugin_destroy;
  rawPlugin.activate = plugin_activate;
  rawPlugin.deactivate = plugin_deactivate;
  rawPlugin.start_processing = plugin_start_processing;
  rawPlugin.stop_processing = plugin_stop_processing;
  rawPlugin.reset = plugin_reset;
  rawPlugin.process = plugin_process;
  rawPlugin.get_extension = plugin_get_extension;
  rawPlugin.on_main_thread = plugin_on_main_thread;
  rawPlugin.plugin_data = reinterpret_cast<void *>(this);
}

}  // namespace erachnid
