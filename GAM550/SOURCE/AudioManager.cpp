#include "Stdafx.h"

Implementation *implementation = nullptr;
AudioManager::AudioManager()
{
	m_Channel = NULL;
}


AudioManager::~AudioManager()
{
}

Implementation::Implementation() {
	studio_system_ = NULL;

	int channel_size_ = 512;
	AudioManager::ErrorCheck(FMOD::Studio::System::create(&studio_system_));
	AudioManager::ErrorCheck(studio_system_->initialize(channel_size_, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));

	system_ = NULL;
	AudioManager::ErrorCheck(studio_system_->getLowLevelSystem(&system_));
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

void AudioManager::ErrorCheck(FMOD_RESULT result_) {
	/*if (result_ != FMOD_OK)
	{
		printf("FMOD ERROR %d", result_);
		return 1;
	}
	return 0;*/

	assert(FMOD_OK == result_);
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

	FMOD::Sound *sound;
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

		//Check again if any error
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
	m_Channel = pchannel;
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

void AudioManager::PauseAudio(bool is_Enabled)
{
	m_Channel->setPaused(true);
}

//Example to call audio 
//INFECT_AUDIOMANAGER.PlaySounds(R"(ASSETS/SOUNDS/rosey.wav)", Vector3(0.0f, 0.0f, 0.0f), INFECT_AUDIOMANAGER.VolumeTodB(0.5f));

//Example to load Audio
//INFECT_AUDIOMANAGER.LoadSound(R"(ASSETS/SOUNDS/rosey.wav)", true, false, false);