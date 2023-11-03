// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ColorUpdater.h"

#include "ParticleData.h"


void ColorUpdater::Update(float, ParticleData* pData)
{
	size_t endId{ pData->countAlive_ };
	for (size_t index{ 0 }; index < endId; ++index)
	{
		pData->colors_[index] = color_;
	}
}


void ColorUpdater::SetColor(const uint color)
{
	color_ = color;
}