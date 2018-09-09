#include "Stdafx.h"
#include "AudioManager.h"


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

	system_ = NULL;
	AudioManager::ErrorCheck(studio_system_->getLowLevelSystem(&system_));
}

int AudioManager::ErrorCheck(FMOD_RESULT result_) {
	if (result_ != FMOD_OK)
	{
		printf("FMOD ERROR %d", result_);
		return 1;
	}
	return 0;
}