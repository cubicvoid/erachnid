#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <sys/time.h>

#include <clap/clap.h>

#include "chomp_impl.hh"


clap_process_status chomp_process(
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
