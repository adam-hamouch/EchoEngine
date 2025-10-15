#include "Resources/Sound.h"

#include <fmod_errors.h>
#include <iostream>

#include "Core/Engine.h"
#include "Sound/AudioEngine.h"

void Sound::LoadResource(const char* path)
{
    _path = path; 

    FMOD::System* system = Engine::GetInstance()->GetAudioEngine()->GetSystem();

    ERRCHECK(system->createSound(path, FMOD_DEFAULT, nullptr, &sound));
}

FMOD::Sound* Sound::GetInternalSound() const
{
    return sound;
}

void Sound::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cerr << "FMOD Error: " << FMOD_ErrorString(result) << std::endl;
    }
}
