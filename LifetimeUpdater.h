#pragma once

#include "ParticleUpdater.h"


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class LifetimeUpdater : public ParticleUpdater
{
public:
	void Update(float deltaTime, ParticleData* pData) override;
};

