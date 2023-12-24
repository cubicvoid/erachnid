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

  float **out = process->audio_outputs[0].data32;
  auto    chans = process->audio_outputs->channel_count;

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

    // This is a simple accumulator of output across our active voices.
    // See saw-voice.h for information on the individual voice.
    for (int ch = 0; ch < chans; ++ch) {
      out[ch][i] = 0.f;
    }
    for (auto &v : voices) {
      if (v.isPlaying()) {
        v.step();
        if (chans >= 2) {
          out[0][i] += v.L;
          out[1][i] += v.R;
        } else if (chans == 1) {
          out[0][i] += (v.L + v.R) * 0.5;
        }
      }
    }
  }
  float **out = process->audio_outputs[0].data32;
  auto    chans = process->audio_outputs->channel_count;

  auto ov = process->out_events;
  for (const auto &[portid, channel, key, note_id] : terminated_voices) {
    clap_event_note evt;
    evt.header.size = sizeof(clap_event_note);
    evt.header.type = (uint16_t)CLAP_EVENT_NOTE_END;
    evt.header.time = process->frames_count - 1;
    evt.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    evt.header.flags = 0;

    evt.port_index = portid;
    evt.channel = channel;
    evt.key = key;
    evt.note_id = note_id;
    evt.velocity = 0.0;

    ov->try_push(ov, &(evt.header));

    // dataCopyForUI.updateCount++;
    // dataCopyForUI.polyphony--;
  }
  terminated_voices.clear();

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
      handleNoteOn(nevt);
    } break;
    case CLAP_EVENT_NOTE_OFF: {
      auto nevt = reinterpret_cast<const clap_event_note *>(hdr);
      handleNoteOff(nevt);
    } break;
    case CLAP_EVENT_NOTE_CHOKE: {
      auto nevt = reinterpret_cast<const clap_event_note *>(hdr);
      handleNoteChoke(nevt);
    } break;
    case CLAP_EVENT_PARAM_VALUE: {
      // TODO: handle params
      // auto v = reinterpret_cast<const clap_event_param_value *>(hdr);
    } break;
    case CLAP_EVENT_PARAM_MOD: {
    } break;
  }
}

StarryVoice *StarryPlugin::chooseNewVoice() {
  for (StarryVoice &v : voices) {
    if (v.state == StarryVoice::OFF) {
      return &v;
    }
  }
  int          idx = rand() % max_voices;
  StarryVoice &v = voices[idx];
  terminated_voices.emplace_back(v.portid, v.channel, v.key, v.note_id);
  return &v;
}

void StarryPlugin::handleNoteOn(const clap_event_note *event) {
  StarryVoice *v = chooseNewVoice();
  activateVoice(v, event);
  // dataCopyForUI.updateCount++;
  // dataCopyForUI.polyphony++;

  // TODO: update UI
}

void StarryPlugin::handleNoteOff(const clap_event_note *event) {
  for (StarryVoice &v : voices) {
    if (v.isPlaying() && v.key == event->key && v.portid == event->port_index &&
        v.channel == event->channel) {
      v.release();
    }
  }
}

void StarryPlugin::handleNoteChoke(const clap_event_note *event) {}

void StarryPlugin::activateVoice(StarryVoice *v, const clap_event_note *event) {
  v->state = StarryVoice::ATTACK;
  v->key = event->key;
  v->portid = event->port_index;
  v->channel = event->channel;
  v->note_id = event->note_id;
}

void StarryPlugin::chokeVoice(StarryVoice *v) {}

}  // namespace erachnid::starry
