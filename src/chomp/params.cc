#include "params.hh"

#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace chomp {

std::vector<Param *> Param::all;
std::vector<Param *> Param::active;

Param::Param(
    ParamID     id,
    std::string name,
    std::string module,
    double      minValue,
    double      maxValue,
    double      defaultValue,
    uint64_t    flags
)
    : value(defaultValue) {
  info.id = id;
  strncpy(info.name, name.c_str(), sizeof(info.name));
  strncpy(info.module, module.c_str(), sizeof(info.module));
  info.min_value = minValue;
  info.max_value = maxValue;
  info.default_value = defaultValue;
  info.flags = flags;

  all.push_back(this);
}

Param::~Param() {
  auto it = std::find(all.begin(), all.end(), this);
  if (it != all.end()) {
    all.erase(it);
  }
  it = std::find(active.begin(), active.end(), this);
  if (it != active.end()) {
    active.erase(it);
  }
}

void Param::Finalize(const Plugin &plugin) {
  active.clear();
  for (auto param : all) {
    if (param->Active(plugin)) {
      active.push_back(param);
    }
  }
}

uint32_t Param::Count() { return static_cast<uint32_t>(active.size()); }

bool Param::GetInfo(uint32_t param_index, clap_param_info_t *param_info) {
  if (param_index >= active.size()) {
    return false;
  }
  const Param &param = *active[param_index];
  memcpy(param_info, &param.info, sizeof(param.info));
  return true;
}

bool Param::GetValue(clap_id param_id, double *out_value) {
  for (const Param *param : active) {
    if (param->info.id == param_id) {
      *out_value = param->GetValue();
      return true;
    }
  }
  return false;
}

bool Param::ValueToText(
    clap_id  param_id,
    double   value,
    char    *out_buffer,
    uint32_t out_buffer_capacity
) {
  for (const Param *param : active) {
    if (param->info.id == param_id) {
      std::string text = param->ValueToText(value);
      strncpy(out_buffer, text.c_str(), out_buffer_capacity);
      return true;
    }
  }
  return false;
}

bool Param::TextToValue(
    clap_id param_id, const char *param_value_text, double *out_value
) {
  for (const Param *param : active) {
    if (param->info.id == param_id) {
      *out_value = param->TextToValue(param_value_text);
      return true;
    }
  }
  return false;
}

void Param::Flush(
    const clap_input_events_t *in, const clap_output_events_t *out
) {}
}  // namespace chomp