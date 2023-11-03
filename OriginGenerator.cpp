// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "OriginGenerator.h"

#include "ParticleData.h"


OriginGenerator::OriginGenerator(float2& position)
	: position_{ position }
{	}


void OriginGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	// Start at emitter (which is where generator is positioned).
	for (size_t index{ startId }; index < endId; ++index)
	{
		pData->origins_[index] = position_;
	}
}