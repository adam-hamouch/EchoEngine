#pragma once
#include "Core/DataStructure/Component.h"
#include "Math/Math.h"
#include "fmod.hpp"

class ISound; 

class SoundComponent : public Component
{
public:
    SoundComponent(); 
    ~SoundComponent() override = default; 

    void SetSound(ISound* sound);
    void Play();
    void Stop();
    void Pause();
    void SetVolume(float v);
    void Set3D(bool is3D);
    void SetLoop(bool loop);
    void SetPosition(const Vector3D& pos);

    ISound* GetSound() const { return sound; }
    bool GetLoop() const { return loop; }
    bool Get3D() const { return is3D; }
    float GetVolume() const { return volume; }
    Vector3D GetPosition() const { return position;  }

    void DisplayComponentInInspector(InspectorUI* inspector) override;

private:
    ISound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
    FMOD::System* system = nullptr;

    bool is3D = true;
    bool loop = false;
    float volume = 1.0f;
    Vector3D position = Vector3D(0.f, 0.f, 0.f); 

    void ERRCHECK(FMOD_RESULT result);
};
