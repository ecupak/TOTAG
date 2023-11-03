#include "precomp.h"
#include "ParticleRewindData.h"


ParticleRewindData::ParticleRewindData()
	: positions_(new float4[50])	
	, radians_(new float2[50])
	, lifetimes_(new float[50])	
	, isAlives_(new bool[50])
{	}


ParticleRewindData::~ParticleRewindData()
{
	free(positions_);

	free(radians_);

	free(lifetimes_);

	free(isAlives_);
}