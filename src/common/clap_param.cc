#include "clap_param.hh"

#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "clap_plugin.hh"

namespace erachnid {

CLAPParam::CLAPParam(
    uint32_t    id,
    std::string name,
    std::string module,
    double      min_value,
    double      max_value,
    double      default_value,
    uint64_t    flags
)
    : _id(id),
      _name(name),
      _module(module),
      _min_value(min_value),
      _max_value(max_value),
      _default_value(default_value),
      _flags(flags) {}
CLAPParam::~CLAPParam() {}

void CLAPParam::GetInfo(clap_param_info_t *param_info) {
  param_info->id = _id;
  param_info->flags = _flags;
  param_info->cookie = reinterpret_cast<void *>(this);
  strncpy(param_info->name, _name.c_str(), sizeof(param_info->name));
  strncpy(param_info->module, _module.c_str(), sizeof(param_info->module));
  param_info->min_value = _min_value;
  param_info->max_value = _max_value;
  param_info->default_value = _default_value;
}

bool CLAPParam::ValueToText(
    double value, char *out_buffer, uint32_t out_buffer_capacity
) {
  if (_flags & CLAP_PARAM_IS_STEPPED) {
    snprintf(
        out_buffer, out_buffer_capacity, "%d", static_cast<int>(value + 0.5)
    );
  } else {
    snprintf(out_buffer, out_buffer_capacity, "%0.3lf", value);
  }
  return true;
}

bool CLAPParam::TextToValue(const char *param_value_text, double *out_value) {
  char *endptr;
  strtod(param_value_text, &endptr);
  return (endptr != param_value_text && *endptr == 0);
}

uint32_t CLAPPlugin::ParamsCount() {
  return static_cast<uint32_t>(_params.size());
}

bool CLAPPlugin::ParamsGetInfo(
    uint32_t param_index, clap_param_info_t *param_info
) {
  if (param_index >= _params.size()) {
    return false;
  }
  _params[param_index]->GetInfo(param_info);
  return true;
}

bool CLAPPlugin::ParamsGetValue(clap_id param_id, double *out_value) {
  auto it = _params_lookup.find(param_id);
  if (it == _params_lookup.end()) {
    return false;
  }
  *out_value = (it->second)->GetValue();
  return true;
}

}  // namespace erachnid