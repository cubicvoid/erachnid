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

  CLAP_ABI void plugin_on_main_thread(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->OnMainThread();
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

  CLAP_ABI void plugin_deactivate(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->Deactivate();
  }

  CLAP_ABI bool plugin_start_processing(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->StartProcessing();
  }

  CLAP_ABI void plugin_stop_processing(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->StopProcessing();
  }

  CLAP_ABI clap_process_status plugin_process(
      const struct clap_plugin *_plugin, const clap_process_t *process
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->Process(process);
  }

  CLAP_ABI void plugin_reset(const struct clap_plugin *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->Reset();
  }

  ///////////////////////
  // clap_plugin_state //
  ///////////////////////

  CLAP_ABI bool plugin_state_save(const clap_plugin_t *_plugin, const clap_ostream_t *stream) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->StateSave(stream);
  }

  CLAP_ABI bool plugin_state_load(const clap_plugin_t *_plugin, const clap_istream_t *stream) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->StateLoad(stream);
  }

  const clap_plugin_state_t s_plugin_state = {
    .save = plugin_state_save,
    .load = plugin_state_load,
  };


  /////////////////////////////
  // clap_plugin_audio_ports //
  /////////////////////////////

  CLAP_ABI uint32_t
  plugin_audio_ports_count(const clap_plugin_t *_plugin, bool is_input) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->AudioPortsCount(is_input);
  }

  CLAP_ABI bool plugin_audio_ports_get(
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

  CLAP_ABI uint32_t
  plugin_note_ports_count(const clap_plugin_t *_plugin, bool is_input) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->NotePortsCount(is_input);
  }

  CLAP_ABI bool plugin_note_ports_get(
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

  CLAP_ABI uint32_t plugin_params_count(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamsCount();
  }

  CLAP_ABI bool plugin_params_get_info(
      const clap_plugin_t *raw_plugin,
      uint32_t             param_index,
      clap_param_info_t   *param_info
  ) {
    CLAPPlugin *plugin =
        reinterpret_cast<CLAPPlugin *>(raw_plugin->plugin_data);
    return plugin->ParamsGetInfo(param_index, param_info);
  }

  CLAP_ABI bool plugin_params_get_value(
      const clap_plugin_t *raw_plugin, clap_id param_id, double *out_value
  ) {
    CLAPPlugin *plugin =
        reinterpret_cast<CLAPPlugin *>(raw_plugin->plugin_data);
    return plugin->ParamsGetValue(param_id, out_value);
  }

  CLAP_ABI bool plugin_params_value_to_text(
      const clap_plugin_t *_plugin,
      clap_id              param_id,
      double               value,
      char                *out_buffer,
      uint32_t             out_buffer_capacity
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamsValueToText(
        param_id, value, out_buffer, out_buffer_capacity
    );
  }

  CLAP_ABI bool plugin_params_text_to_value(
      const clap_plugin_t *_plugin,
      clap_id              param_id,
      const char          *param_value_text,
      double              *out_value
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->ParamsTextToValue(param_id, param_value_text, out_value);
  }

  CLAP_ABI void plugin_params_flush(
      const clap_plugin_t        *_plugin,
      const clap_input_events_t  *in,
      const clap_output_events_t *out
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    plugin->ParamsFlush(in, out);
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

  CLAP_ABI bool plugin_gui_is_api_supported(
      const clap_plugin_t *_plugin, const char *api, bool is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIIsAPISupported(api, is_floating);
  }

  CLAP_ABI bool plugin_gui_get_preferred_api(
      const clap_plugin_t *_plugin, const char **api, bool *is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIGetPreferredAPI(api, is_floating);
  }

  CLAP_ABI bool plugin_gui_create(
      const clap_plugin_t *_plugin, const char *api, bool is_floating
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUICreate(api, is_floating);
  }

  CLAP_ABI void plugin_gui_destroy(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIDestroy();
  }

  CLAP_ABI bool plugin_gui_set_scale(
      const clap_plugin_t *_plugin, double scale
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetScale(scale);
  }

  CLAP_ABI bool plugin_gui_get_size(
      const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIGetSize(width, height);
  }

  CLAP_ABI bool plugin_gui_can_resize(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUICanResize();
  }

  CLAP_ABI bool plugin_gui_get_resize_hints(
      const clap_plugin_t *_plugin, clap_gui_resize_hints_t *hints
  ) {
    // CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin
    // *>(_plugin->plugin_data);

    // TODO
    return false;
  }

  CLAP_ABI bool plugin_gui_adjust_size(
      const clap_plugin_t *_plugin, uint32_t *width, uint32_t *height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIAdjustSize(width, height);
  }

  CLAP_ABI bool plugin_gui_set_size(
      const clap_plugin_t *_plugin, uint32_t width, uint32_t height
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetSize(width, height);
  }

  CLAP_ABI bool plugin_gui_set_parent(
      const clap_plugin_t *_plugin, const clap_window_t *window
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetParent(window);
  }

  CLAP_ABI bool plugin_gui_set_transient(
      const clap_plugin_t *_plugin, const clap_window_t *window
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUISetTransient(window);
  }

  CLAP_ABI void plugin_gui_suggest_title(
      const clap_plugin_t *plugin, const char *title
  ) {
    // TODO: low priority since this is floating only
  }

  CLAP_ABI bool plugin_gui_show(const clap_plugin_t *_plugin) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);
    return plugin->GUIShow();
  }

  CLAP_ABI bool plugin_gui_hide(const clap_plugin_t *_plugin) {
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

  CLAP_ABI const void *plugin_get_extension(
      const struct clap_plugin *_plugin, const char *id
  ) {
    CLAPPlugin *plugin = reinterpret_cast<CLAPPlugin *>(_plugin->plugin_data);

    if (!strcmp(id, CLAP_EXT_PARAMS)) {
      return &s_plugin_params;
    }
    if (!strcmp(id, CLAP_EXT_STATE)) {
      return &s_plugin_state;
    }
    if (plugin->AudioPortsEnabled() && !strcmp(id, CLAP_EXT_AUDIO_PORTS)) {
      return &s_plugin_audio_ports;
    }
    if (plugin->NotePortsEnabled() && !strcmp(id, CLAP_EXT_NOTE_PORTS)) {
      return &s_plugin_note_ports;
    }

    if (plugin->GUIEnabled() && !strcmp(id, CLAP_EXT_GUI)) {
      return &s_plugin_gui;
    }
    // TODO: add support to CLAP_EXT_PARAMS
    return NULL;
  }

}  // namespace

void CLAPPlugin::InitRawPlugin(const clap_plugin_descriptor_t *desc) {
  _raw_plugin.desc = desc;
  _raw_plugin.init = plugin_init;
  _raw_plugin.destroy = plugin_destroy;
  _raw_plugin.activate = plugin_activate;
  _raw_plugin.deactivate = plugin_deactivate;
  _raw_plugin.start_processing = plugin_start_processing;
  _raw_plugin.stop_processing = plugin_stop_processing;
  _raw_plugin.reset = plugin_reset;
  _raw_plugin.process = plugin_process;
  _raw_plugin.get_extension = plugin_get_extension;
  _raw_plugin.on_main_thread = plugin_on_main_thread;
  _raw_plugin.plugin_data = reinterpret_cast<void *>(this);
}

}  // namespace erachnid
