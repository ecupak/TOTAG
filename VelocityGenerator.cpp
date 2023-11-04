// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "VelocityGenerator.h"

#include "ParticleData.h"


void VelocityGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	for (size_t index{ startId }; index < endId; ++index)
	{
		float4& ray{ pData->positions_[index] };
		
		// x and y velocities.
		ray.w = 9.0f;
		ray.z = 5.0f + (RandomFloat() * 15.0f) ;
	}
}