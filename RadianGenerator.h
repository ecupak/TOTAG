#pragma once

#include "ParticleGenerator.h"


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class RadianGenerator : public ParticleGenerator
{
public:
	void Generate(float deltaTime, ParticleData* pData, size_t startId, size_t endId) override;
};
