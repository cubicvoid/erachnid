#include <clap/clap.h>
#include <sys/time.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "chomp.hh"

namespace erachnid::chomp {

void ChompPlugin::NoteOn(const clap_event_note_t *note) { envTarget = 1.0; }

void ChompPlugin::NoteOff(const clap_event_note_t *note) { envTarget = 0.0; }

bool ChompPlugin::StartProcessing() {
  processing = true;
  return true;
}

void ChompPlugin::StopProcessing() { processing = false; }

clap_process_status ChompPlugin::Process(const clap_process_t *process) {
  const uint32_t frameCount = process->frames_count;
  const uint32_t eventCount = process->in_events->size(process->in_events);
  uint32_t       eventIndex = 0;
  uint32_t       nextEventFrame = eventCount > 0 ? 0 : frameCount;

  /*const double freq = 2.0 * M_PI * 440.0 / 44100.0;
  double       pos = static_cast<double>(sampleCount) * freq;
  for (uint32_t i = 0; i < frameCount; i++) {
    sampleCount++;
    process->audio_outputs->data32[0][i] = 0.2f * sin(pos);
    process->audio_outputs->data32[1][i] = 0.2f * sin(pos);
    pos += freq;
  }
  process->audio_outputs->constant_mask = 0;*/
  for (uint32_t i = 0; i < frameCount;) {
    // handle every events that happens at the frame "i"
    while (eventIndex < eventCount && nextEventFrame == i) {
      const clap_event_header_t *hdr =
          process->in_events->get(process->in_events, eventIndex);
      if (hdr->time != i) {
        nextEventFrame = hdr->time;
        break;
      }

      ProcessEvent(hdr);
      ++eventIndex;

      if (eventIndex == eventCount) {
        // we reached the end of the event list
        nextEventFrame = frameCount;
        break;
      }
    }

    // process every samples until the next event
    for (; i < nextEventFrame; ++i) {
      // fetch input samples
      const float in_l = process->audio_inputs[0].data32[0][i];
      const float in_r = process->audio_inputs[0].data32[1][i];

      const float out_l = in_l * envVolume;
      const float out_r = in_r * envVolume;

      // store output samples
      process->audio_outputs[0].data32[0][i] = out_l;
      process->audio_outputs[0].data32[1][i] = out_r;

      envVolume += (envTarget - envVolume) * 0.5;
    }
    if (std::fabs(envTarget - envVolume) < 0.00001) {
      envVolume = envTarget;
    }
  }
  sampleCount += frameCount;
  if (sampleCount > 10000 && sampleCount - frameCount <= 10000) {
    // one-time restart for testing
    // host->request_restart(host);
    host->request_callback(host);
    return CLAP_PROCESS_SLEEP;
  }

  return CLAP_PROCESS_CONTINUE;
}

void ChompPlugin::ProcessEvent(const clap_event_header_t *hdr) {
  if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
    switch (hdr->type) {
      case CLAP_EVENT_NOTE_ON: {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        // const Voice             &v = GetVoice(*ev);
        noteOnCount++;
        NoteOn(ev);
        // TODO: handle note on
        break;
      }

      case CLAP_EVENT_NOTE_OFF: {
        const clap_event_note_t *ev =
            reinterpret_cast<const clap_event_note_t *>(hdr);
        noteOffCount++;
        NoteOff(ev);
        // TODO: handle note off
        break;
      }

      case CLAP_EVENT_NOTE_CHOKE: {
        // const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        //  TODO: handle note choke
        break;
      }

      case CLAP_EVENT_NOTE_EXPRESSION: {
        const clap_event_note_expression_t *ev =
            (const clap_event_note_expression_t *)hdr;
        // TODO: handle note expression
        break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
        const clap_event_param_value_t *ev =
            (const clap_event_param_value_t *)hdr;
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
        midiCount++;
        // TODO: handle MIDI event
        break;
      }

      case CLAP_EVENT_MIDI_SYSEX: {
        const clap_event_midi_sysex_t *ev =
            (const clap_event_midi_sysex_t *)hdr;
        midiCount++;
        // TODO: handle MIDI Sysex event
        break;
      }

      case CLAP_EVENT_MIDI2: {
        const clap_event_midi2_t *ev = (const clap_event_midi2_t *)hdr;
        // TODO: handle MIDI2 event
        midiCount++;
        break;
      }
    }
  }
}

}  // namespace erachnid::chomp