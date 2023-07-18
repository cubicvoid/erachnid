#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <clap/clap.h>

extern "C" const clap_plugin_descriptor_t chomp_plug_desc = {
   .clap_version = CLAP_VERSION_INIT,
   .id = "me.faec.erachnid.bitesu",
   .name = "bites u",
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
} chomp_plug_t;

static bool chomp_plug_init(const struct clap_plugin *plugin) {
   chomp_plug_t *plug = static_cast<chomp_plug_t*>(plugin->plugin_data);

   // Fetch host's extensions here
   // Make sure to check that the interface functions are not null pointers
   plug->host_log = (const clap_host_log_t *)plug->host->get_extension(plug->host, CLAP_EXT_LOG);
   plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
   plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
   plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
   return true;
}

static void chomp_plug_destroy(const struct clap_plugin *plugin) {
   chomp_plug_t *plug = static_cast<chomp_plug_t*>(plugin->plugin_data);
   free(plug);
}

static bool chomp_plug_activate(const struct clap_plugin *plugin,
                             double                    sample_rate,
                             uint32_t                  min_frames_count,
                             uint32_t                  max_frames_count) {
   return true;
}

static void chomp_plug_deactivate(const struct clap_plugin *plugin) {}

static bool chomp_plug_start_processing(const struct clap_plugin *plugin) { return true; }

static void chomp_plug_stop_processing(const struct clap_plugin *plugin) {}

static void chomp_plug_reset(const struct clap_plugin *plugin) {}

static clap_process_status chomp_plug_process(const struct clap_plugin *plugin,
                                           const clap_process_t     *process) {
   /*chomp_plug_t     *plug = plugin->plugin_data;
   const uint32_t nframes = process->frames_count;
   const uint32_t nev = process->in_events->size(process->in_events);
   uint32_t       ev_index = 0;
   uint32_t       next_ev_frame = nev > 0 ? 0 : nframes;

   for (uint32_t i = 0; i < nframes;) {
      // handle every events that happens at the frame "i"
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

      // process every samples until the next event
      for (; i < next_ev_frame; ++i) {
         // fetch input samples
         const float in_l = process->audio_inputs[0].data32[0][i];
         const float in_r = process->audio_inputs[0].data32[1][i];

         // TODO: process samples, here we simply swap left and right channels
         const float out_l = in_r;
         const float out_r = in_l;

         // store output samples
         process->audio_outputs[0].data32[0][i] = out_l;
         process->audio_outputs[0].data32[1][i] = out_r;
      }
   }*/

   return CLAP_PROCESS_CONTINUE;
}

static const void *chomp_plug_get_extension(const struct clap_plugin *plugin, const char *id) {
   /*if (!strcmp(id, CLAP_EXT_LATENCY))
      return &s_my_plug_latency;
   if (!strcmp(id, CLAP_EXT_AUDIO_PORTS))
      return &s_my_plug_audio_ports;
   if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
      return &s_my_plug_note_ports;
   if (!strcmp(id, CLAP_EXT_STATE))
      return &s_my_plug_state;*/
   // TODO: add support to CLAP_EXT_PARAMS
   return NULL;
}

static void chomp_plug_on_main_thread(const struct clap_plugin *plugin) {}


extern "C" clap_plugin_t *chomp_plug_create(const clap_host_t *host) {
   chomp_plug_t *p = static_cast<chomp_plug_t*>(calloc(1, sizeof(*p)));
   p->host = host;
   p->plugin.desc = &chomp_plug_desc;
   p->plugin.plugin_data = p;
   p->plugin.init = chomp_plug_init;
   p->plugin.destroy = chomp_plug_destroy;
   p->plugin.activate = chomp_plug_activate;
   p->plugin.deactivate = chomp_plug_deactivate;
   p->plugin.start_processing = chomp_plug_start_processing;
   p->plugin.stop_processing = chomp_plug_stop_processing;
   p->plugin.reset = chomp_plug_reset;
   p->plugin.process = chomp_plug_process;
   p->plugin.get_extension = chomp_plug_get_extension;
   p->plugin.on_main_thread = chomp_plug_on_main_thread;

   // Don't call into the host here

   return &p->plugin;
}
