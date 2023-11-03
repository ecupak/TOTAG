// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "RadianGenerator.h"

#include "ParticleData.h"


void RadianGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	for (size_t index{ startId }; index < endId; ++index)
	{
		pData->radians_[index] = float2{ 2 * PI * RandomFloat(), (RandomUInt() & 1) == 0 ? -1.0f : 1.0f};
	}
}