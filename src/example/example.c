#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <clap/clap.h>

const clap_plugin_descriptor_t s_my_plug_desc = {
   .clap_version = CLAP_VERSION_INIT,
   .id = "me.faec.erachnid.test",
   .name = "erachnid test",
   .vendor = "cubicvoid",
   .url = "https://faec.me",
   .manual_url = "https://your-domain.com/your-plugin/manual",
   .support_url = "https://your-domain.com/support",
   .version = "0.0.1",
   .description = "~fae~",
   .features = (const char *[]){CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

typedef struct {
   clap_plugin_t                   plugin;
   const clap_host_t              *host;
   const clap_host_latency_t      *host_latency;
   const clap_host_log_t          *host_log;
   const clap_host_thread_check_t *host_thread_check;
   const clap_host_state_t        *host_state;

   uint32_t latency;
} my_plug_t;


/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////

static uint32_t my_plug_audio_ports_count(const clap_plugin_t *plugin, bool is_input) {
   // We just declare 1 audio input and 1 audio output
   return 1;
}

static bool my_plug_audio_ports_get(const clap_plugin_t    *plugin,
                                    uint32_t                index,
                                    bool                    is_input,
                                    clap_audio_port_info_t *info) {
   if (index > 0)
      return false;
   info->id = 0;
   snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
   info->channel_count = 2;
   info->flags = CLAP_AUDIO_PORT_IS_MAIN;
   info->port_type = CLAP_PORT_STEREO;
   info->in_place_pair = CLAP_INVALID_ID;
   return true;
}

static const clap_plugin_audio_ports_t s_my_plug_audio_ports = {
   .count = my_plug_audio_ports_count,
   .get = my_plug_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t my_plug_note_ports_count(const clap_plugin_t *plugin, bool is_input) {
   // We just declare 1 note input
   return 1;
}

static bool my_plug_note_ports_get(const clap_plugin_t   *plugin,
                                   uint32_t               index,
                                   bool                   is_input,
                                   clap_note_port_info_t *info) {
   if (index > 0)
      return false;
   info->id = 0;
   snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
   info->supported_dialects =
      CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
   info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
   return true;
}

static const clap_plugin_note_ports_t s_my_plug_note_ports = {
   .count = my_plug_note_ports_count,
   .get = my_plug_note_ports_get,
};

//////////////////
// clap_latency //
//////////////////

uint32_t my_plug_latency_get(const clap_plugin_t *plugin) {
   my_plug_t *plug = plugin->plugin_data;
   return plug->latency;
}

static const clap_plugin_latency_t s_my_plug_latency = {
   .get = my_plug_latency_get,
};

////////////////
// clap_state //
////////////////

bool my_plug_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
   my_plug_t *plug = plugin->plugin_data;
   // TODO: write the state into stream
   return true;
}

bool my_plug_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream) {
   my_plug_t *plug = plugin->plugin_data;
   // TODO: read the state from stream
   return true;
}

static const clap_plugin_state_t s_my_plug_state = {
   .save = my_plug_state_save,
   .load = my_plug_state_load,
};

/////////////////
// clap_plugin //
/////////////////

static bool my_plug_init(const struct clap_plugin *plugin) {
   my_plug_t *plug = plugin->plugin_data;

   // Fetch host's extensions here
   // Make sure to check that the interface functions are not null pointers
   plug->host_log = (const clap_host_log_t *)plug->host->get_extension(plug->host, CLAP_EXT_LOG);
   plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
   plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
   plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
   return true;
}

static void my_plug_destroy(const struct clap_plugin *plugin) {
   my_plug_t *plug = plugin->plugin_data;
   free(plug);
}

static bool my_plug_activate(const struct clap_plugin *plugin,
                             double                    sample_rate,
                             uint32_t                  min_frames_count,
                             uint32_t                  max_frames_count) {
   return true;
}

static void my_plug_deactivate(const struct clap_plugin *plugin) {}

static bool my_plug_start_processing(const struct clap_plugin *plugin) { return true; }

static void my_plug_stop_processing(const struct clap_plugin *plugin) {}

static void my_plug_reset(const struct clap_plugin *plugin) {}

static void my_plug_process_event(my_plug_t *plug, const clap_event_header_t *hdr) {
   if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (hdr->type) {
      case CLAP_EVENT_NOTE_ON: {
         const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
         // TODO: handle note on
         break;
      }

      case CLAP_EVENT_NOTE_OFF: {
         const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
         // TODO: handle note off
         break;
      }

      case CLAP_EVENT_NOTE_CHOKE: {
         const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
         // TODO: handle note choke
         break;
      }

      case CLAP_EVENT_NOTE_EXPRESSION: {
         const clap_event_note_expression_t *ev = (const clap_event_note_expression_t *)hdr;
         // TODO: handle note expression
         break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
         const clap_event_param_value_t *ev = (const clap_event_param_value_t *)hdr;
         // TODO: handle parameter change
         break;
      }

      case CLAP_EVENT_PARAM_MOD: {
         const clap_event_param_mod_t *ev = (const clap_event_param_mod_t *)hdr;
         // TODO: handle parameter modulation
         break;
      }

      case CLAP_EVENT_TRANSPORT: {
         const clap_event_transport_t *ev = (const clap_event_transport_t *)hdr;
         // TODO: handle transport event
         break;
      }

      case CLAP_EVENT_MIDI: {
         const clap_event_midi_t *ev = (const clap_event_midi_t *)hdr;
         // TODO: handle MIDI event
         break;
      }

      case CLAP_EVENT_MIDI_SYSEX: {
         const clap_event_midi_sysex_t *ev = (const clap_event_midi_sysex_t *)hdr;
         // TODO: handle MIDI Sysex event
         break;
      }

      case CLAP_EVENT_MIDI2: {
         const clap_event_midi2_t *ev = (const clap_event_midi2_t *)hdr;
         // TODO: handle MIDI2 event
         break;
      }
      }
   }
}

static clap_process_status my_plug_process(const struct clap_plugin *plugin,
                                           const clap_process_t     *process) {
   my_plug_t     *plug = plugin->plugin_data;
   const uint32_t nframes = process->frames_count;
   const uint32_t nev = process->in_events->size(process->in_events);
   uint32_t       ev_index = 0;
   uint32_t       next_ev_frame = nev > 0 ? 0 : nframes;

   for (uint32_t i = 0; i < nframes;) {
      /* handle every events that happrens at the frame "i" */
      while (ev_index < nev && next_ev_frame == i) {
         const clap_event_header_t *hdr = process->in_events->get(process->in_events, ev_index);
         if (hdr->time != i) {
            next_ev_frame = hdr->time;
            break;
         }

         my_plug_process_event(plug, hdr);
         ++ev_index;

         if (ev_index == nev) {
            // we reached the end of the event list
            next_ev_frame = nframes;
            break;
         }
      }

      /* process every samples until the next event */
      for (; i < next_ev_frame; ++i) {
         // fetch input samples
         const float in_l = process->audio_inputs[0].data32[0][i];
         const float in_r = process->audio_inputs[0].data32[1][i];

         /* TODO: process samples, here we simply swap left and right channels */
         const float out_l = in_r;
         const float out_r = in_l;

         // store output samples
         process->audio_outputs[0].data32[0][i] = out_l;
         process->audio_outputs[0].data32[1][i] = out_r;
      }
   }

   return CLAP_PROCESS_CONTINUE;
}

static const void *my_plug_get_extension(const struct clap_plugin *plugin, const char *id) {
   if (!strcmp(id, CLAP_EXT_LATENCY))
      return &s_my_plug_latency;
   if (!strcmp(id, CLAP_EXT_AUDIO_PORTS))
      return &s_my_plug_audio_ports;
   if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
      return &s_my_plug_note_ports;
   if (!strcmp(id, CLAP_EXT_STATE))
      return &s_my_plug_state;
   // TODO: add support to CLAP_EXT_PARAMS
   return NULL;
}

static void my_plug_on_main_thread(const struct clap_plugin *plugin) {}

clap_plugin_t *my_plug_create(const clap_host_t *host) {
   my_plug_t *p = calloc(1, sizeof(*p));
   p->host = host;
   p->plugin.desc = &s_my_plug_desc;
   p->plugin.plugin_data = p;
   p->plugin.init = my_plug_init;
   p->plugin.destroy = my_plug_destroy;
   p->plugin.activate = my_plug_activate;
   p->plugin.deactivate = my_plug_deactivate;
   p->plugin.start_processing = my_plug_start_processing;
   p->plugin.stop_processing = my_plug_stop_processing;
   p->plugin.reset = my_plug_reset;
   p->plugin.process = my_plug_process;
   p->plugin.get_extension = my_plug_get_extension;
   p->plugin.on_main_thread = my_plug_on_main_thread;

   // Don't call into the host here

   return &p->plugin;
}
