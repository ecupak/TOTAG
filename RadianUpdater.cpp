// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "RadianUpdater.h"

#include "ParticleData.h"


void RadianUpdater::Update(float deltaTime, ParticleData* pData)
{
	float amountToAdvance{ deltaTime * 1.4f * PI };

	size_t endId{ pData->countAlive_ };
	for (size_t index{ 0 }; index < endId; ++index)
	{
		pData->radians_[index].x += amountToAdvance;
	}
}