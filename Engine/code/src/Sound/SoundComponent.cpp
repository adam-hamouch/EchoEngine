#include "Sound/SoundComponent.h"

#include <fmod_errors.h>

#include "Core/UI/UI.h"
#include "Core/Engine.h"

#include "Resources/Sound.h"

#include "Sound/AudioEngine.h"

void SoundComponent::ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        std::cerr << "FMOD Error: " << FMOD_ErrorString(result) << std::endl;
    }
}

SoundComponent::SoundComponent()
{
    system = Engine::GetInstance()->GetAudioEngine()->GetSystem(); 
}

void SoundComponent::SetSound(ISound* _sound)
{
    sound = _sound; 
}

void SoundComponent::Play()
{
    if (channel)
    {
        bool isPlaying = false;
        channel->isPlaying(&isPlaying);

        if (isPlaying)
            channel->stop(); 
    }

    ERRCHECK(system->playSound(sound->GetInternalSound(), nullptr, true, &channel));

    if (is3D)
    {
        FMOD_VECTOR fmodPos = { position.x, position.y, position.z };
        channel->set3DAttributes(&fmodPos, nullptr);
    }

    ERRCHECK(channel->setVolume(volume));
    ERRCHECK(channel->setPaused(false));
}


void SoundComponent::Pause()
{
    if (channel) {
        bool paused;
        channel->getPaused(&paused);
        channel->setPaused(!paused);
    }
}

void SoundComponent::SetPosition(const Vector3D& pos)
{
    position = pos; 
    if (channel && is3D) {

        FMOD_VECTOR fmodPos = { pos.x, pos.y, pos.z };
        channel->set3DAttributes(&fmodPos, nullptr);
    }
}

void SoundComponent::SetVolume(float v)
{
    volume = v; 
    if (channel) {
        channel->setVolume(v);
    }
}

void SoundComponent::SetLoop(bool loopFlag)
{
    loop = loopFlag; 
    if (sound) {
        sound->GetInternalSound()->setMode(loopFlag ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    }
}

void SoundComponent::Set3D(bool is3DFlag)
{
    is3D = is3DFlag; 
    if (sound) {
        sound->GetInternalSound()->setMode(is3DFlag ? FMOD_3D : FMOD_2D);
    }
}

void SoundComponent::Stop()
{
    if (channel) {
        channel->stop();
        channel = nullptr;
    }
}

void SoundComponent::DisplayComponentInInspector(InspectorUI* inspector)
{
    inspector->SoundDisplay();
}