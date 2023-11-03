// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ColorGenerator.h"

#include "ParticleData.h"


ColorGenerator::ColorGenerator(uint& color)
	: color_{color}
{	}


void ColorGenerator::Generate(float, ParticleData* pData, size_t startId, size_t endId)
{
	for (size_t index{ startId }; index < endId; ++index)
	{
		pData->colors_[index] = color_;
	}
}


void ColorGenerator::SetColor(const uint color)
{
	color_ = color;
}