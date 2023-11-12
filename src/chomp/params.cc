#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp_impl.hh"

namespace chomp {

// std::vector<Param *> Param::all;
// std::vector<Param *> Param::active;

void Plugin::AddParam(
    ParamID     id,
    std::string name,
    std::string module,
    double      minValue,
    double      maxValue,
    double      defaultValue,
    uint64_t    flags
) {
  clap_param_info_t info;

  info.id = id;
  strncpy(info.name, name.c_str(), sizeof(info.name));
  strncpy(info.module, module.c_str(), sizeof(info.module));
  info.min_value = minValue;
  info.max_value = maxValue;
  info.default_value = defaultValue;
  info.flags = flags;
  paramsAll.push_back(new Param(info, defaultValue));
}

// Called on plugin activation
void Plugin::RefreshParameters() {
  /*paramsActive.clear();
  for (auto param : paramsAll) {
    if (param->Active(*this)) {
      paramsActive.push_back(param);
    }
  }*/
  // TODO: if we ever have context-sensitive parameters, this is where to
  // disable the unused ones.
  paramsActive = paramsAll;
}

// uint32_t Param::Count() { return static_cast<uint32_t>(active.size()); }

uint32_t Plugin::ParamCount() {
  uint32_t result = static_cast<uint32_t>(paramsActive.size());
  Log("ParamCount() -> %d", result);
  return result;
}

bool Plugin::ParamGetInfo(uint32_t param_index, clap_param_info_t *param_info) {
  if (param_index >= paramsActive.size()) {
    return false;
  }
  const Param &param = *paramsActive[param_index];
  memcpy(param_info, &param.info, sizeof(param.info));
  return true;
}

bool Plugin::ParamGetValue(clap_id param_id, double *out_value) {
  for (const Param *param : paramsActive) {
    if (param->info.id == param_id) {
      *out_value = param->value;
      return true;
    }
  }
  return false;
}

bool Plugin::ParamValueToText(
    clap_id  param_id,
    double   value,
    char    *out_buffer,
    uint32_t out_buffer_capacity
) {
  for (const Param *param : paramsActive) {
    if (param->info.id == param_id) {
      std::string text = std::to_string(value);
      strncpy(out_buffer, text.c_str(), out_buffer_capacity);
      return true;
    }
  }
  return false;
}

bool Plugin::ParamTextToValue(
    clap_id param_id, const char *param_value_text, double *out_value
) {
  for (const Param *param : paramsActive) {
    if (param->info.id == param_id) {
      *out_value = std::stod(param_value_text);
      return true;
    }
  }
  return false;
}

void Plugin::ParamFlush(
    const clap_input_events_t *in, const clap_output_events_t *out
) {}
}  // namespace chomp