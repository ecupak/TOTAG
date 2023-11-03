// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "LifetimeGenerator.h"

#include "ParticleData.h"


void LifetimeGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	// Last for 1 second.
	for (size_t index{ startId }; index < endId; ++index)
	{
		pData->lifetimes_[index] = 10.0f;
	}
}