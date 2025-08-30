#pragma once

#include <vector>

#include <clap/clap.h>


namespace erachnid::common {


class VoiceTracking {
public:

	VoiceTracking(int voice_count);

	void NoteOn(const clap_event_note_t *note);
	void NoteOff(const clap_event_note_t *note);
	void NoteChoke(const clap_event_note_t *note);

	void BeginProcess();
	void Advance(uint32_t sample_count);
	void EndProcess();

private:

	struct Voice {
		Voice() : active(false), held(false) { }

		bool Match(const clap_event_note_t *note);

		clap_event_note_t note;

		bool active;
		bool held;
	};


	std::vector<Voice> voices;
	uint32_t process_pos;


};



}