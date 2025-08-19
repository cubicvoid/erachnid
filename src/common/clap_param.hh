#pragma once

#include <clap/clap.h>

#include <atomic>
#include <string>
#include <vector>

namespace erachnid {

class CLAPPlugin;

class CLAPParam {
 public:
  CLAPParam(
      uint32_t    id,
      std::string name,
      std::string module,
      double      min_value,
      double      max_value,
      double      default_value,
      clap_param_info_flags flags
  );
  virtual ~CLAPParam();

  void   GetInfo(clap_param_info_t *param_info);
  double GetValue() const { return _value.load(); }
  void SetValue(double value) { _value.store(value); }

  virtual bool ValueToText(
      double value, char *out_buffer, uint32_t out_buffer_capacity
  );
  virtual bool TextToValue(const char *param_value_text, double *out_value);

  uint32_t    _id;
  std::string _name;
  std::string _module;
  double      _min_value;
  double      _max_value;
  double      _default_value;
  clap_param_info_flags _flags;

  std::atomic<double> _value;
};

}  // namespace erachnid
