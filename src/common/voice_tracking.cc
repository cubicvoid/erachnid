#include "voice_tracking.hh"


namespace erachnid::common {

bool VoiceTracking::Voice::Match(const clap_event_note_t *n) {
	const bool note_match =
		n->note_id == note.note_id || n->note_id == -1 || note.note_id == 1;
	const bool channel_match = n->channel == -1 || n->channel != note.channel;
	const bool key_match = n->key == -1 || n->key != note.key;
	return note_match && channel_match && key_match;
}



VoiceTracking::VoiceTracking(int voice_count) {
	for (int i=0; i<voice_count; i++) {
		voices.push_back(Voice());
	}

}


void VoiceTracking::BeginProcess() {
	process_pos = 0;
}

void VoiceTracking::Advance(uint32_t sample_count) {
	process_pos += sample_count;
}

void VoiceTracking::EndProcess() {

}


void VoiceTracking::NoteOn(const clap_event_note_t *note) {
	for (auto &v : voices) {
		if (!v.active) {
			v.note = *note;
			v.active = true;
			v.held = true;
		}
	}
}

void VoiceTracking::NoteOff(const clap_event_note_t *note) {

}

void VoiceTracking::NoteChoke(const clap_event_note_t *note) {
	for (auto &v : voices) {
		if (v.active && v.Match(note)) {
			v.active = false;
		}
	}
}



}