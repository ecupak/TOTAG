// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ParticleData.h"


ParticleData::ParticleData(size_t maxCount)
{
	Generate(maxCount);
}


ParticleData::~ParticleData()
{
	// Keep getting "not allocated by _aligned routines, use free()" error on closing.
	free(origins_);
	free(positions_);
	free(radians_);
	free(colors_);
	free(lifetimes_);
	free(isAlives_);
}


void ParticleData::Generate(size_t maxSize)
{
	// Size arrays to hold all particles.
	origins_ = new float2[maxSize];
	positions_ = new float4[maxSize];
	radians_ = new float2[maxSize];
	colors_ = new uint[maxSize];
	lifetimes_ = new float[maxSize];
	isAlives_ = new bool[maxSize];

	// Set size info.
	count_ = maxSize;

	// Ensure all start out as 'dead'.
	for (size_t index{ 0 }; index < count_; ++index)
	{
		isAlives_[index] = false;
	}
}


// Deactivate a particle. Replaces its data in array with last active particle in list.
void ParticleData::Kill(size_t id)
{
	isAlives_[id] = false;
	SwapData(id, countAlive_ - 1);	
	--countAlive_;
}


// Mark as active.
void ParticleData::Wake(size_t id)
{
	isAlives_[id] = true;
	++countAlive_;
}


// Only need to assign the currently alive item to the newly dead item's position.
void ParticleData::SwapData(size_t replace, size_t replacement)
{
	origins_[replace] = origins_[replacement];
	positions_[replace] = positions_[replacement];
	radians_[replace] = radians_[replacement];
	colors_[replace] = colors_[replacement];
	lifetimes_[replace] = lifetimes_[replacement];
	isAlives_[replace] = isAlives_[replacement];
}