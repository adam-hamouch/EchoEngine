#include "Sound/AudioEngine.h"

#include <fmod_errors.h>
#include <iostream>

#include "LowRenderer/Camera.h"

void AudioEngine::Init()
{
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
        std::cerr << "FMOD init error: " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }

    system->setSoftwareFormat(192000, FMOD_SPEAKERMODE_STEREO, 0);

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        std::cerr << "FMOD system init error: " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }

    std::cout << "[FMOD] Audio system initialized.\n";
}

void AudioEngine::Shutdown() {
    if (system) {
        system->close();
        system->release();
        system = nullptr;
        std::cout << "[FMOD] Audio system shut down.\n";
    }
}

void AudioEngine::Update(Camera* camera) const {
    if (system)
    {
        system->update();

        FMOD_VECTOR listenerPos = { camera->eye.x, camera->eye.y, camera->eye.z };
        FMOD_VECTOR listenerVel = { 0.0f, 0.0f, 0.0f }; 
        FMOD_VECTOR forward = { -camera->f.x, -camera->f.y, -camera->f.z };
        FMOD_VECTOR up = { camera->u.x, camera->u.y, camera->u.z };

        system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &forward, &up);
    }
}

void AudioEngine::SetSampleRate(int sampleRate) const
{
    system->setSoftwareFormat(sampleRate, FMOD_SPEAKERMODE_DEFAULT, 0);
}

FMOD::System* AudioEngine::GetSystem() const
{
    return system;
}
