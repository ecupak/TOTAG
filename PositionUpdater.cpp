// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "PositionUpdater.h"

#include "ParticleData.h"


PositionUpdater::PositionUpdater(float2& position)
	: position_{ position }
{	}


// As particles rise, they move left and right based on a sine wave calculation.
void PositionUpdater::Update(float deltaTime, ParticleData* pData)
{
	size_t endId{ pData->countAlive_ };
	for (size_t index{ 0 }; index < endId; ++index)
	{
		// x and y are position. w and z are velocities.
		float4& position{ pData->positions_[index] };

		position.x = cos(pData->radians_[index].x) * 8.0f;
		position.y -= position.z * deltaTime;

		// Mark for removal.
		if (position.y < -48.0f)
		{
			pData->isAlives_[index] = false;
		}
	}
}