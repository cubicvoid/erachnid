#include <clap/clap.h>
#include <sys/time.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "plugin.hh"

namespace erachnid::skeleton {

void Plugin::NoteOn(const clap_event_note_t *note) {}

void Plugin::NoteOff(const clap_event_note_t *note) {}

clap_process_status Plugin::Process(const clap_process_t *process) {
  const uint32_t frameCount = process->frames_count;
  const uint32_t eventCount = process->in_events->size(process->in_events);
  uint32_t       eventIndex = 0;
  const clap_event_header_t *hdr =
      eventCount > 0 ? process->in_events->get(process->in_events, 0) : nullptr;
  uint32_t nextEventFrame = eventCount > 0 ? hdr->time : frameCount;

  for (uint32_t i = 0; i < frameCount;) {
    // handle events for frame i
    while (nextEventFrame == i) {
      ProcessEvent(hdr);
      ++eventIndex;

      if (eventIndex == eventCount) {
        // we reached the end of the event list
        nextEventFrame = frameCount;
        break;
      }
      hdr = process->in_events->get(process->in_events, eventIndex);
      nextEventFrame = hdr->time;
    }

    // process every samples until the next event
    for (; i < nextEventFrame; ++i) {
      // fetch input samples
      const float in_l = process->audio_inputs[0].data32[0][i];
      const float in_r = process->audio_inputs[0].data32[1][i];

      const float out_l = in_l;
      const float out_r = in_r;

      // store output samples
      process->audio_outputs[0].data32[0][i] = out_l;
      process->audio_outputs[0].data32[1][i] = out_r;
    }
  }
  return CLAP_PROCESS_CONTINUE;
}

void Plugin::ProcessEvent(const clap_event_header_t *hdr) {
  if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
    switch (hdr->type) {
      case CLAP_EVENT_NOTE_ON: {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        NoteOn(ev);
        break;
      }

      case CLAP_EVENT_NOTE_OFF: {
        const clap_event_note_t *ev =
            reinterpret_cast<const clap_event_note_t *>(hdr);
        NoteOff(ev);
        break;
      }

      case CLAP_EVENT_NOTE_CHOKE: {
        // const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        //  TODO: handle note choke
        break;
      }

      case CLAP_EVENT_NOTE_EXPRESSION: {
        // const clap_event_note_expression_t *ev =
        //     (const clap_event_note_expression_t *)hdr;
        // TODO: handle note expression
        break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
        // const clap_event_param_value_t *ev =
        //     (const clap_event_param_value_t *)hdr;
        // TODO: handle parameter change
        break;
      }

      case CLAP_EVENT_PARAM_MOD: {
        // const clap_event_param_mod_t *ev = (const clap_event_param_mod_t
        // *)hdr;
        //  TODO: handle parameter modulation
        break;
      }

      case CLAP_EVENT_TRANSPORT: {
        // const clap_event_transport_t *ev = (const clap_event_transport_t
        // *)hdr;
        //  TODO: handle transport event
        break;
      }

      case CLAP_EVENT_MIDI: {
        // const clap_event_midi_t *ev = (const clap_event_midi_t *)hdr;
        // TODO: handle MIDI event
        break;
      }

      case CLAP_EVENT_MIDI_SYSEX: {
        // const clap_event_midi_sysex_t *ev =
        //     (const clap_event_midi_sysex_t *)hdr;
        // TODO: handle MIDI Sysex event
        break;
      }

      case CLAP_EVENT_MIDI2: {
        // const clap_event_midi2_t *ev = (const clap_event_midi2_t *)hdr;
        //  TODO: handle MIDI2 event
        break;
      }
    }
  }
}

}  // namespace erachnid::skeleton