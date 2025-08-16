#include <clap/clap.h>
#include <sys/time.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "plugin.hh"

using nlohmann::json;

namespace erachnid::scan {

void Plugin::NoteOn(const clap_event_note_t *note) {}

void Plugin::NoteOff(const clap_event_note_t *note) {}

/*struct SerializedEvent {
  uint32_t time;
  uint16_t space_id;'s
  uint16_t type;
};

struct SerializedProcessCall {
  uint32_t frames_count;
  std::vector<SerializedEvent> events;
};*/

json jsonFromHeader(const clap_event_header_t *hdr) {
  json j;
  j["size"] = hdr->size;
  j["time"] = hdr->time;
  j["space_id"] = hdr->space_id;
  j["type"] = hdr->type;
  j["flags"] = hdr->flags;
  return j;
}

json jsonFromEventNote(const clap_event_note_t *note) {
  json j;
  j["channel"] = static_cast<int>(note->channel);
  j["key"] = static_cast<int>(note->key);
  j["note_id"] = static_cast<int>(note->note_id);
  j["port_index"] = static_cast<int>(note->port_index);
  j["velocity"] = static_cast<int>(note->velocity);
  return j;
}

json jsonFromNoteExpression(const clap_event_note_expression_t *note_expression) {
  json j;
  j["expression_id"] = note_expression->expression_id;
  j["note_id"] = note_expression->note_id;
  j["port_index"] = note_expression->port_index;
  j["channel"] = note_expression->channel;
  j["key"] = note_expression->key;
  j["value"] = note_expression->value;
  return j;
}

json jsonFromParamValue(const clap_event_param_value_t *param_value) {
  json j;
  j["param_id"] = param_value->param_id;
  j["note_id"] = param_value->note_id;
  j["port_index"] = param_value->port_index;
  j["channel"] = param_value->channel;
  j["key"] = param_value->key;
  j["value"] = param_value->value;
  return j;
}

json jsonFromParamMod(const clap_event_param_mod_t *param_mod) {
  json j;
  j["param_id"] = param_mod->param_id;
  j["note_id"] = param_mod->note_id;
  j["port_index"] = param_mod->port_index;
  j["channel"] = param_mod->channel;
  j["key"] = param_mod->key;
  j["amount"] = param_mod->amount;
  return j;
}

double doubleFromBeatTime(clap_beattime value) {
  return static_cast<double>(value) /
    static_cast<double>(CLAP_BEATTIME_FACTOR);
}

double doubleFromSecTime(clap_sectime value) {
  return static_cast<double>(value) /
    static_cast<double>(CLAP_SECTIME_FACTOR);
}

json jsonFromTransport(const clap_event_transport_t *transport) {
  json j;
  j["flags"] = transport->flags;
  j["song_pos_beats"] = doubleFromBeatTime(transport->song_pos_beats);
  j["song_pos_seconds"] = doubleFromSecTime(transport->song_pos_seconds);
  j["tempo"] = transport->tempo;
  j["tempo_inc"] = transport->tempo_inc;
  j["loop_start_beats"] = doubleFromBeatTime(transport->loop_start_beats);
  j["loop_end_beats"] = doubleFromBeatTime(transport->loop_end_beats);
  j["loop_start_seconds"] = doubleFromSecTime(transport->loop_start_seconds);
  j["loop_end_seconds"] = doubleFromSecTime(transport->loop_end_seconds);
  j["bar_start"] = doubleFromBeatTime(transport->bar_start);
  j["bar_number"] = transport->bar_number;
  j["tsig_num"] = transport->tsig_num;
  j["tsig_denom"] = transport->tsig_denom;
}

json jsonFromEvent(const clap_event_header_t *hdr) {
  json j;

  j["header"] = jsonFromHeader(hdr);
  if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
    switch (hdr->type) {
      case CLAP_EVENT_NOTE_ON:
      case CLAP_EVENT_NOTE_OFF:
      case CLAP_EVENT_NOTE_CHOKE:
       {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        j["note"] = jsonFromEventNote(ev);
        break;
      }

      case CLAP_EVENT_NOTE_EXPRESSION: {
        const clap_event_note_expression_t *ev =
            (const clap_event_note_expression_t *)hdr;
        j["note_expression"] = jsonFromNoteExpression(ev);
        break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
        const clap_event_param_value_t *ev =
            (const clap_event_param_value_t *)hdr;
          j["param_value"] = jsonFromParamValue(ev);
        break;
      }

      case CLAP_EVENT_PARAM_MOD: {
        const clap_event_param_mod_t *ev = (const clap_event_param_mod_t *)hdr;
        j["param_mod"] = jsonFromParamMod(ev);
        break;
      }

      case CLAP_EVENT_TRANSPORT: {
        const clap_event_transport_t *ev = (const clap_event_transport_t *)hdr;
        j["transport"] = jsonFromTransport(ev);
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
  return j;
}

json jsonFromInputEvents(const clap_input_events_t *in_events) {
  std::vector<json> events;
  const uint32_t eventCount = in_events->size(in_events);
  for (uint32_t i = 0; i < eventCount; i++) {
    const clap_event_header_t *hdr = in_events->get(in_events, i);
    events.push_back(jsonFromEvent(hdr));
  }
  return events;
}

json jsonFromProcess(const clap_process_t *process) {
  json j;
  j["steady_time"] = process->steady_time;
  j["frames_count"] = process->frames_count;
  if (process->transport != nullptr) {
    j["transport"] = jsonFromTransport(process->transport);
  }
  j["events"] = jsonFromInputEvents(process->in_events);
  return j;
}

clap_process_status Plugin::Process(const clap_process_t *process) {
  const uint32_t frameCount = process->frames_count;
  const uint32_t eventCount = process->in_events->size(process->in_events);
  uint32_t       eventIndex = 0;
  const clap_event_header_t *hdr =
      eventCount > 0 ? process->in_events->get(process->in_events, 0) : nullptr;
  uint32_t nextEventFrame = eventCount > 0 ? hdr->time : frameCount;

  json j;
  j["process"] = jsonFromProcess(process);
  entries.push_back(j);
  // process every samples until the next event
  for (uint32_t i = 0; i < frameCount; ++i) {
    // fetch input samples
    const float in_l = process->audio_inputs[0].data32[0][i];
    const float in_r = process->audio_inputs[0].data32[1][i];

    const float out_l = in_l;
    const float out_r = in_r;

    // store output samples
    process->audio_outputs[0].data32[0][i] = out_l;
    process->audio_outputs[0].data32[1][i] = out_r;
  }

  RequestMainThreadCallback();
  return CLAP_PROCESS_CONTINUE;
}

void Plugin::ParamsFlush(
  const clap_input_events_t  *in, const clap_output_events_t *out
) {
}


}  // namespace erachnid::skeleton