#pragma once

#include <clap/clap.h>

#include <string>
#include <vector>

namespace erachnid {

class CLAPPlugin;

enum ParamID {
  ParamIDRats,
  ParamIDAttack,
};

/*class Param {
 private:
  static std::vector<Param *> all;
  static std::vector<Param *> active;

  clap_param_info_t info;
  double            value;

 public:
  Param(
      ParamID     id,
      std::string name,
      std::string module,
      double      minValue,
      double      maxValue,
      double      defaultValue,
      uint64_t    flags
  );
  virtual ~Param();

  // CLAP params extension API
  // static uint32_t Count();
  static bool GetInfo(uint32_t param_index, clap_param_info_t *param_info);
  static bool GetValue(clap_id param_id, double *out_value);
  static bool ValueToText(
      clap_id  param_id,
      double   value,
      char    *out_buffer,
      uint32_t out_buffer_capacity
  );
  static bool TextToValue(
      clap_id param_id, const char *param_value_text, double *out_value
  );
  static void Flush(
      const clap_input_events_t *in, const clap_output_events_t *out
  );

  // call Finalize when the plugin is about to activate
  static void Finalize(const Plugin &plugin);

  virtual bool   Active(const Plugin &plugin) const { return true; }
  virtual double GetValue() const { return value; }

  virtual std::string ValueToText(double value) const {
    return std::to_string(value);
  }
  virtual double TextToValue(const std::string &text) const {
    return std::stod(text);
  }
};*/

}  // namespace erachnid