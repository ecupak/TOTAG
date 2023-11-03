// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ParticleTrasher.h"

#include "ParticleData.h"


void ParticleTrasher::Cleanup(ParticleData* pData)
{
	if (pData->countAlive_ == 0)
	{
		return;
	}

	size_t startId{ pData->countAlive_ - 1 };

	for (size_t index{ startId };;)
	{
		// Remove dead particle.
		if (pData->isAlives_[index] == false)
		{
			pData->Kill(index);
		}

		// Custom flow to deal with stepping backwards with a size_t data type (can't be negative).
		if (index-- == 0)
		{
			break;
		}
	}
}