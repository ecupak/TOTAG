#pragma once

class ParticleData;


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
class ParticleTrasher
{
public:
	void Cleanup(ParticleData* pData);
};

