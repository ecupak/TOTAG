// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ParticleRenderer.h"

#include "ParticleData.h"


void ParticleRenderer::Draw(Surface* target, int x, int y, ParticleData* pData, const uint color)
{
	size_t endId{ pData->countAlive_ };
	for (size_t index{ 0 }; index < endId; ++index)
	{
		float4& offsetPosition{ pData->positions_[index] };
				
		target->Plot(static_cast<int>(x + offsetPosition.x), static_cast<int>(y + offsetPosition.y), color);
	}
}