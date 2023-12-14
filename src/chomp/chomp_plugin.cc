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
#include "params.hh"
#include "reaper_plugin.h"

namespace erachnid::chomp {

extern const clap_plugin_descriptor_t plugin_desc;

ChompPlugin::ChompPlugin(const clap_host_t *_host)
    : CLAPPlugin(_host, &plugin_desc) {
  static int count = 0;
  pluginID = count++;

  Log("plugin_create()");

  // init_plugin(&plugin, reinterpret_cast<void *>(this));
  // TODO: reactivate the gui
  // gui.reset(GUIWrapper::New(this));

  AddParam(
      ParamIDRats, "rats", "something", 0, 100, 50,
      CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE
  );
  AddParam(
      ParamIDAttack, "attack", "something else", 0.0, 1.0, 0.0,
      CLAP_PARAM_IS_AUTOMATABLE
  );
  RefreshParameters();
}

}