#include <clap/clap.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

namespace chomp {

uint32_t Plugin::ParamsCount() {
  flog(host, CLAP_LOG_INFO, "chomp_params_count()");
  return 1;
}

bool Plugin::ParamsGetInfo(
    uint32_t param_index, clap_param_info_t *param_info
) {
  char buf[256];
  snprintf(buf, sizeof(buf), "chomp_params_get_info(index: %d)", param_index);
  flog(host, CLAP_LOG_INFO, buf);
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

bool Plugin::ParamsGetValue(clap_id param_id, double *out_value) {
  *out_value = 50;
  return true;
}

bool Plugin::ParamsValueToText(
    clap_id  param_id,
    double   value,
    char    *out_buffer,
    uint32_t out_buffer_capacity
) {
  snprintf(out_buffer, out_buffer_capacity, "%d rats", (int)value);
  return true;
}

bool Plugin::ParamsTextToValue(
    clap_id param_id, const char *param_value_text, double *out_value
) {
  *out_value = 50.0;
  return true;
}

void Plugin::ParamsFlush(
    const clap_input_events_t *in, const clap_output_events_t *out
) {}

}  // namespace chomp