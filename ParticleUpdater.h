#pragma once

class ParticleData;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleUpdater
{
public:
	virtual ~ParticleUpdater() { }

	virtual void Update(float deltaTime, ParticleData* pData) = 0;
};

