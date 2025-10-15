#pragma once
#include "ISound.h"

class Sound : public ISound
{
public:
	void LoadResource(const char* path) override;

	FMOD::Sound* GetInternalSound() const override;

private:
	FMOD::Sound* sound = nullptr;
	FMOD_MODE mode = FMOD_DEFAULT;

	void ERRCHECK(FMOD_RESULT result);
};
