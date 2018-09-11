#include "Stdafx.h"

Implementation *implementation = nullptr;
AudioManager::AudioManager()
{
}


AudioManager::~AudioManager()
{
}

Implementation::Implementation() {
	studio_system_ = NULL;

	AudioManager::ErrorCheck(FMOD::Studio::System::create(&studio_system_));
	AudioManager::ErrorCheck(studio_system_->initialize(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	/*system_ = NULL;
	AudioManager::ErrorCheck(studio_system_->getLowLevelSystem(&system_));*/
}

Implementation::~Implementation()
{
	AudioManager::ErrorCheck(studio_system_->unloadAll());
	AudioManager::ErrorCheck(studio_system_->release());
}

void Implementation::Update()
{
	//Check for the audio to finish playing
	vector<ChannelMap::iterator> stopped_channel;
	for (auto it = channel_map_.begin(), it_end = channel_map_.end(); it != it_end; ++it)
	{
		bool is_playing = false;
		it->second->isPlaying(&is_playing);
		if (!is_playing)
		{
			stopped_channel.push_back(it);
		}
	}
	//Erase From the audio map
	for (auto& it : stopped_channel)
	{
		channel_map_.erase(it);
	}
	//Update the system
	AudioManager::ErrorCheck(studio_system_->update());
}

void AudioManager::Init()
{
	implementation = new Implementation();
}


void AudioManager::Update()
{
	implementation->Update();

}

int AudioManager::ErrorCheck(FMOD_RESULT result_) {
	if (result_ != FMOD_OK)
	{
		printf("FMOD ERROR %d", result_);
		return 1;
	}
	return 0;
}

void AudioManager::LoadSound( const string & audio_name, bool is_audio_3D, bool is_audio_looping, bool is_audio_streaming)
{
	auto found_it = implementation->sound_map_.find(audio_name);
	if (found_it != implementation->sound_map_.end())
		return;

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= is_audio_3D ? FMOD_3D : FMOD_2D;
	mode |= is_audio_looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= is_audio_streaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* sound = nullptr;
	AudioManager::ErrorCheck(implementation->system_->createSound(audio_name.c_str(), mode, nullptr, &sound));



	if (sound) {
		implementation->sound_map_[audio_name] = sound;
	}

}

void AudioManager::UnLoadSound(const string & audio_name)
{

	auto found_it = implementation->sound_map_.find(audio_name);
	if (found_it == implementation->sound_map_.end())
		return;

	AudioManager::ErrorCheck(found_it->second->release());
	implementation->sound_map_.erase(found_it);

}

int AudioManager::PlaySounds(const string & Sound_name, const Vector3 & Position, float Volume_play)
{

	float frequency;
	int channel_id = implementation->next_channel_id_++;
	auto found_it = implementation->sound_map_.find(Sound_name);
	
	if (found_it == implementation->sound_map_.end())
	{
		//Load Sound
		LoadSound(Sound_name);
		found_it = implementation->sound_map_.find(Sound_name);

		if (found_it == implementation->sound_map_.end())
		{
			return channel_id;
		}
	}

	FMOD::Channel* pchannel = nullptr;
	AudioManager::ErrorCheck(implementation->system_->playSound(found_it->second, nullptr, true , &pchannel));
	 
	if (pchannel)
	{
		FMOD_MODE currmode;
		found_it->second->getMode(&currmode);
		if (currmode &FMOD_3D)
		{
			FMOD_VECTOR position = VectorToFmod(Position);
			AudioManager::ErrorCheck(pchannel->set3DAttributes(&position, nullptr));
		}

		AudioManager::ErrorCheck(pchannel->setVolume(dbToVolume(Volume_play)));
		AudioManager::ErrorCheck(pchannel->setPaused(false));

		implementation->channel_map_[channel_id] = pchannel;

	}
	return channel_id;
}

FMOD_VECTOR AudioManager::VectorToFmod(const Vector3 position)
{
	FMOD_VECTOR vector;
	vector.x = position.x;
	vector.y = position.y;
	vector.z = position.z;
	return vector;

}

float AudioManager::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioManager::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}

void AudioManager::SetChannel3dPosition(int channel_id, const Vector3 pos)
{
	auto found_it = implementation->channel_map_.find(channel_id);
	if (found_it == implementation->channel_map_.end())
		return;

	FMOD_VECTOR position = VectorToFmod(pos);
	AudioManager::ErrorCheck(found_it->second->set3DAttributes(&position, NULL));
}

void AudioManager::SetChannelVolume(int channel_id, float volume_db)
{
	auto found_it = implementation->channel_map_.find(channel_id);
	if (found_it == implementation->channel_map_.end())
		return;

	AudioManager::ErrorCheck(found_it->second->setVolume(dbToVolume(volume_db)));
}

void AudioManager::LoadEvent(const std::string & event_name)
{
	auto found_it = implementation->events_.find(event_name);
	if (found_it != implementation->events_.end())
		return;
	FMOD::Studio::EventDescription* event_description = NULL;
	AudioManager::ErrorCheck(implementation->studio_system_->getEvent(event_name.c_str(), &event_description));
	if (event_description)
	{
		FMOD::Studio::EventInstance *event_instance = NULL;
		AudioManager::ErrorCheck(event_description->createInstance(&event_instance));
		if (event_instance)
		{
			implementation->events_[event_name] = event_instance;
		}
	}
}

void AudioManager::PlayEvent(const string & event_name)
{
	auto found_it = implementation->events_.find(event_name);
	if (found_it == implementation->events_.end())
	{
		LoadEvent(event_name);
		found_it = implementation->events_.find(event_name);
		if (found_it == implementation->events_.end())
			return;
	}
	found_it->second->start();
}

void AudioManager::StopEvent(const string & event_name, bool immediate)
{
	auto found_it = implementation->events_.find(event_name);
	if (found_it == implementation->events_.end())
		return;

	FMOD_STUDIO_STOP_MODE mode;
	mode = immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioManager::ErrorCheck(found_it->second->stop(mode));
}


