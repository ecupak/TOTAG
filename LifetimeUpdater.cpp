// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "LifetimeUpdater.h"

#include "ParticleData.h"


void LifetimeUpdater::Update(float deltaTime, 
	ParticleData* pData)
{
	// Reduce life.
	size_t endId{ pData->countAlive_ };
	for (size_t index{ 0 }; index < endId; ++index)
	{
		pData->lifetimes_[index] -= deltaTime;

		// Mark for removal.
		if (pData->lifetimes_[index] < 0.0f)
		{
			pData->isAlives_[index] = false;
		}
	}
}