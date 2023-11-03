#pragma once

#include "ParticleUpdater.h"


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class PositionUpdater : public ParticleUpdater
{
public:
	PositionUpdater(float2& position);

	void Update(float deltaTime, ParticleData* pData) override;


private:
	float2& position_;
};