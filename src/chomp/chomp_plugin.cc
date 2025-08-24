#include "chomp_plugin.hh"

#include <clap/clap.h>
#include <sys/time.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>

#ifdef DARWIN
#include "gui/chomp_gui_darwin.h"
#endif

// #include "bridge.hh"
// #include "gui/gui.hh"
#include "clap_param.hh"
#include "reaper_plugin.h"

enum { PARAM_RATS, PARAM_ATTACK };

namespace erachnid::chomp {
extern const clap_plugin_descriptor_t plugin_desc;

ChompPlugin::ChompPlugin(const clap_host_t *_host)
    : CLAPPlugin(_host, &plugin_desc) {

  AddParam(std::shared_ptr<CLAPParam>(new CLAPParam(
      PARAM_RATS, "rats", "something", 0, 100, 50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  )));
  AddParam(std::shared_ptr<CLAPParam>(new CLAPParam(
      PARAM_ATTACK, "attack", "something else", 0.0, 1.0, 0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  )));
}

}  // namespace erachnid::chomp
