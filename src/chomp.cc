#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <sys/time.h>

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

class ChompPlugin {
public:
   ChompPlugin(const clap_host_t *_host);

   FILE *logFile;

   void flog(const clap_host_t *host, clap_log_severity sev, const char *msg) {
      if (logFile != nullptr) {
         struct timeval tv;
         gettimeofday(&tv, nullptr);
         time_t t = tv.tv_sec;
         struct tm *info = localtime(&t);
         printf("%s",asctime (info));

         char buf[512];
         snprintf(buf, sizeof(buf), "(%d) %s\n", sev, msg);
         fwrite(buf, 1, strlen(buf), logFile);
         fflush(logFile);
      }
   }

   clap_plugin_t                   plugin;
   const clap_host_t              *host;
   const clap_host_latency_t      *host_latency;
   const clap_host_thread_check_t *host_thread_check;
   const clap_host_state_t        *host_state;

   void(CLAP_ABI *log)(const clap_host_t *host, clap_log_severity severity, const char *msg);
   
   uint32_t latency;
};

void CLAP_ABI nilLog(const clap_host_t *host, clap_log_severity severity, const char *msg) {
}

static bool chomp_plug_init(const struct clap_plugin *plugin) {
   ChompPlugin *plug = static_cast<ChompPlugin*>(plugin->plugin_data);
   plug->logFile = fopen("/Users/fae/chomp.log", "wb");

   // Fetch host's extensions here
   // Make sure to check that the interface functions are not null pointers
   const clap_host_log_t* ext_log = static_cast<const clap_host_log_t*>(
      plug->host->get_extension(plug->host, CLAP_EXT_LOG));
   if (ext_log != nullptr && ext_log->log != nullptr) {
      plug->log = ext_log->log;
   }
   plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
   plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
   plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
   return true;
}

static void chomp_plug_destroy(const struct clap_plugin *_plugin) {
   ChompPlugin *plugin = static_cast<ChompPlugin*>(_plugin->plugin_data);
   if (plugin->logFile != nullptr) {
      fclose(plugin->logFile);
   }
   delete plugin;
}

static bool chomp_plug_activate(
   const struct clap_plugin *_plugin,
   double                    sample_rate,
   uint32_t                  min_frames_count,
   uint32_t                  max_frames_count
) {
   
   ChompPlugin *plugin = static_cast<ChompPlugin*>(_plugin->plugin_data);
   plugin->flog(plugin->host, CLAP_LOG_INFO, "hi fae");
   //plugin->log(plugin->host, CLAP_LOG_FATAL, "testing host log (the plugin just activated)");
   return true;
}

static void chomp_plug_deactivate(const struct clap_plugin *plugin) {}

static bool chomp_plug_start_processing(const struct clap_plugin *plugin) { return true; }

static void chomp_plug_stop_processing(const struct clap_plugin *plugin) {}

static void chomp_plug_reset(const struct clap_plugin *plugin) {}

static clap_process_status chomp_plug_process(
   const struct clap_plugin *plugin,
   const clap_process_t     *process
) {
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
   ChompPlugin *p = new ChompPlugin(host);
   // Don't call into the host here
   return &p->plugin;
}


ChompPlugin::ChompPlugin(const clap_host_t *_host) : host(_host), log(nilLog) {
   plugin.desc = &chomp_plug_desc;
   plugin.init = chomp_plug_init;
   plugin.destroy = chomp_plug_destroy;
   plugin.activate = chomp_plug_activate;
   plugin.deactivate = chomp_plug_deactivate;
   plugin.start_processing = chomp_plug_start_processing;
   plugin.stop_processing = chomp_plug_stop_processing;
   plugin.reset = chomp_plug_reset;
   plugin.process = chomp_plug_process;
   plugin.get_extension = chomp_plug_get_extension;
   plugin.on_main_thread = chomp_plug_on_main_thread;
   plugin.plugin_data = this;
}