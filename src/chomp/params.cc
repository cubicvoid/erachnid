#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <sys/time.h>

#include <clap/clap.h>

#include "chomp_impl.hh"


static uint32_t chomp_params_count(const clap_plugin_t *_plugin) {
  ChompPlugin *plugin = static_cast<ChompPlugin*>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, "chomp_params_count()");
  return 1;
}

static bool chomp_params_get_info(
	const clap_plugin_t *_plugin,
  uint32_t             param_index,
  clap_param_info_t   *param_info
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_params_get_info(index: %d)", param_index);
  ChompPlugin *plugin = static_cast<ChompPlugin*>(_plugin->plugin_data);
  plugin->flog(plugin->host, CLAP_LOG_INFO, buf);
	if (param_index > 0) {
		return false;
	}
	strncpy(param_info->name, "rats", sizeof(param_info->name));
	param_info->min_value = 1;
	param_info->max_value = 1000;
	param_info->flags = CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE;
	param_info->default_value = 100;
	param_info->id = 0;
	strncpy(param_info->module, "gay/rats", sizeof(param_info->module));
	return true;
}

bool chomp_params_get_value(const clap_plugin_t *plugin, clap_id param_id, double *out_value) {
	*out_value = 50;
	return true;
}

bool chomp_params_value_to_text(
	const clap_plugin_t *plugin,
  clap_id              param_id,
  double               value,
  char                *out_buffer,
  uint32_t             out_buffer_capacity
) {
	snprintf(out_buffer, out_buffer_capacity, "%d rats", (int)value);
	return true;
}

bool chomp_params_text_to_value(
	const clap_plugin_t *plugin,
  clap_id              param_id,
  const char          *param_value_text,
  double              *out_value
) {
	*out_value = 50.0;
	return true;
}

void chomp_params_flush(
	const clap_plugin_t        *plugin,
  const clap_input_events_t  *in,
  const clap_output_events_t *out
) {

}


const clap_plugin_params_t s_chomp_params = {
   .count = chomp_params_count,
	 .get_info = chomp_params_get_info,
	 .get_value = chomp_params_get_value,
	 .value_to_text = chomp_params_value_to_text,
	 .text_to_value = chomp_params_text_to_value,
	 .flush = chomp_params_flush,
};

   // Copies the parameter's info to param_info. Returns true on success.
   // [main-thread]
   /*bool(CLAP_ABI *get_info)(const clap_plugin_t *plugin,
                            uint32_t             param_index,
                            clap_param_info_t   *param_info);

   // Writes the parameter's current value to out_value. Returns true on success.
   // [main-thread]
   bool(CLAP_ABI *get_value)(const clap_plugin_t *plugin, clap_id param_id, double *out_value);

   // Fills out_buffer with a null-terminated UTF-8 string that represents the parameter at the
   // given 'value' argument. eg: "2.3 kHz". Returns true on success. The host should always use
   // this to format parameter values before displaying it to the user. [main-thread]
   bool(CLAP_ABI *value_to_text)(const clap_plugin_t *plugin,
                                 clap_id              param_id,
                                 double               value,
                                 char                *out_buffer,
                                 uint32_t             out_buffer_capacity);

   // Converts the null-terminated UTF-8 param_value_text into a double and writes it to out_value.
   // Returns true on success. The host can use this to convert user input into a parameter value.
   // [main-thread]
   bool(CLAP_ABI *text_to_value)(const clap_plugin_t *plugin,
                                 clap_id              param_id,
                                 const char          *param_value_text,
                                 double              *out_value);

   // Flushes a set of parameter changes.
   // This method must not be called concurrently to clap_plugin->process().
   //
   // Note: if the plugin is processing, then the process() call will already achieve the
   // parameter update (bi-directional), so a call to flush isn't required, also be aware
   // that the plugin may use the sample offset in process(), while this information would be
   // lost within flush().
   //
   // [active ? audio-thread : main-thread]
   void(CLAP_ABI *flush)(const clap_plugin_t        *plugin,
                         const clap_input_events_t  *in,
                         const clap_output_events_t *out);*/