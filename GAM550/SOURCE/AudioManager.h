#pragma once

using namespace std;

struct Implementation {
	Implementation();
	~Implementation() {};

	void Update();

	FMOD::Studio::System *studio_system_;
	FMOD::System* system_;
	FMOD_RESULT result_;

	int next_channel_id_ = 0;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;
	typedef map<int, float> FrequencyMap;

	BankMap banks_;
	EventMap events_;
	SoundMap sound_map_;
	ChannelMap channel_map_;
	FrequencyMap channel_frequencies_;
};


class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static int ErrorCheck(FMOD_RESULT result);
};

