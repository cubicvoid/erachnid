#include "starry_plugin.hh"

namespace erachnid::starry {
extern const clap_plugin_descriptor_t plugin_desc;

StarryPlugin::StarryPlugin(const clap_host_t *_host)
    : CLAPPlugin(_host, &plugin_desc) {}

clap_process_status StarryPlugin::Process(const clap_process_t *process) {
  const uint32_t frames_count = process->frames_count;
  const uint32_t event_count = process->in_events->size(process->in_events);
  uint32_t       ev_index = 0;
  uint32_t       next_ev_frame = event_count > 0 ? 0 : frames_count;

  for (uint32_t i = 0; i < frames_count;) {
    while (ev_index < event_count && next_ev_frame == i) {
      const clap_event_header_t *hdr =
          process->in_events->get(process->in_events, ev_index);
      if (hdr->time != i) {
        next_ev_frame = hdr->time;
        break;
      }

      ProcessEvent(hdr);
      ++ev_index;

      if (ev_index == event_count) {
        // we reached the end of the event list
        next_ev_frame = frames_count;
        break;
      }
    }
  }
  return CLAP_PROCESS_CONTINUE;
}

void StarryPlugin::ProcessEvent(const clap_event_header_t *hdr) {
  if (hdr->space_id != CLAP_CORE_EVENT_SPACE_ID) {
    return;
  }
  switch (hdr->type) {
    case CLAP_EVENT_MIDI: {
      // TODO: handle midi
      // auto mevt = reinterpret_cast<const clap_event_midi *>(hdr);
    }
    case CLAP_EVENT_NOTE_ON: {
      auto nevt = reinterpret_cast<const clap_event_note *>(hdr);
      handleNoteOn(nevt->port_index, nevt->channel, nevt->key, nevt->note_id);
    } break;
    case CLAP_EVENT_NOTE_OFF: {
      auto nevt = reinterpret_cast<const clap_event_note *>(hdr);
      handleNoteOff(nevt->port_index, nevt->channel, nevt->key);
    } break;
    case CLAP_EVENT_PARAM_VALUE: {
      // TODO: handle params
      // auto v = reinterpret_cast<const clap_event_param_value *>(hdr);
    } break;
  }
}

void StarryPlugin::handleNoteOn(
    int port_index, int channel, int key, int noteid
) {
  bool foundVoice{false};
  for (auto &v : voices) {
    if (v.state == SawDemoVoice::OFF) {
      activateVoice(v, port_index, channel, key, noteid);
      foundVoice = true;
      break;
    }
  }

  if (!foundVoice) {
    // We could steal oldest. If you want to do that toss in a PR to add age
    // to the voice I guess. This is just a demo synth though.
    auto  idx = rand() % max_voices;
    auto &v = voices[idx];
    terminatedVoices.emplace_back(v.portid, v.channel, v.key, v.note_id);
    activateVoice(v, port_index, channel, key, noteid);
  }

  dataCopyForUI.updateCount++;
  dataCopyForUI.polyphony++;

  if (editor) {
    auto r = ToUI();
    r.type = ToUI::MIDI_NOTE_ON;
    r.id = (uint32_t)key;
    toUiQ.try_enqueue(r);
  }
}

void StarryPlugin::handleNoteOff(int port_index, int channel, int n) {}

}  // namespace erachnid::starry
