#pragma once

class ParticleData;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleGenerator
{
public:
	virtual ~ParticleGenerator() {}

	virtual void Generate(float deltaTime, ParticleData* pData, size_t startId, size_t endId) = 0;
};

