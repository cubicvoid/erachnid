#include "clap_param.hh"

#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "clap_plugin.hh"

namespace erachnid {

// std::vector<Param *> Param::all;
// std::vector<Param *> Param::active;

// uint32_t Param::Count() { return static_cast<uint32_t>(active.size()); }

CLAPParam::CLAPParam(
    uint32_t    id,
    std::string name,
    std::string module,
    double      min_value,
    double      max_value,
    double      default_value,
    uint64_t    flags
) {}
CLAPParam::~CLAPParam() {}

void CLAPParam::GetInfo(clap_param_info_t *param_info) {}

bool CLAPParam::ValueToText(
    double value, char *out_buffer, uint32_t out_buffer_capacity
) {}

bool CLAPParam::TextToValue(const char *param_value_text, double *out_value) {}

uint32_t CLAPPlugin::ParamsCount() {
  uint32_t result = 0;  // static_cast<uint32_t>(paramsActive.size());
  return result;
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