#pragma once
#include <fmod.hpp>
#include "Utils/Engine.h"
class Camera; 

class ENGINE_API AudioEngine {
public:
	void Init();
	void Shutdown();
	void Update(Camera* camera) const;
	void SetSampleRate(int sampleRate) const;

	FMOD::System* GetSystem() const;

private:
	FMOD::System* system = nullptr;
};
