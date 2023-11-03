// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "PositionGenerator.h"

#include "ParticleData.h"


PositionGenerator::PositionGenerator(float2 position)
	: position_{ position }
{	}


void PositionGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	// Start at emitter (which is where generator is positioned).
	for (size_t index{ startId }; index < endId; ++index)
	{
		float4& ray{ pData->positions_[index] };
		
		ray.x = 0.0f;// position_.x;
		ray.y = 0.0f;// position_.y;
	}
}