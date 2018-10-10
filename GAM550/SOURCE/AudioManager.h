#pragma once


#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


using namespace std;
class Subscriber;

/*
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float n_X,float n_Y,float n_Z)
	{
		x = n_X;
		y = n_Y;
		z = n_Z;
	}

	void SetVector3(Vector3D POS)
	{
		Vector3 &pos_here = *this;
		pos_here.x = POS.x;
		pos_here.y = POS.y;
		pos_here.z = POS.z;

	}
};

*/


struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System *studio_system_;
	FMOD::System* system_;
	FMOD_RESULT result_;

	int next_channel_id_ = 0;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<int, float> FrequencyMap;

	EventMap events_;
	SoundMap sound_map_;
	ChannelMap channel_map_;
	FrequencyMap channel_frequencies_;
};


class AudioManager : public Subscriber
{
public:
	AudioManager();
	~AudioManager();

	Implementation impl_;

	static void Init();
	static void Update();
	static void ErrorCheck(FMOD_RESULT result);


	//MANAGING SOUNDS
	void LoadSound( const string &audio_name, bool is_audio_3D = true, bool is_audio_looping = false, bool is_audio_streaming = false);
	void UnLoadSound(const string &audio_name);
	 int PlaySounds(const string& Sound_name, const Vector3D& Position, float Volume_play);

	//VECTOR CONVERSIONS
	FMOD_VECTOR VectorToFmod(const Vector3D position);

	//CONVERSIONS
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	
	//CHANNEL PROPERTIES
	void SetChannel3dPosition(int channel_id, const Vector3D pos);
	void SetChannelVolume(int channel_id, float volume_db);
	
	//PAUSE AUDIO
	void PauseAudio(bool is_Enabled);

	//3D AUDIO LISTENER 
	void Set3dListener(const Vector3D& pos, const Vector3D& look, const Vector3D& up, const Vector3D& velocity);

	//TESTING
	void TestingAudio();

private:
	FMOD::Channel* m_Channel;

};


#endif