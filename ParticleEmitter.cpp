// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "ParticleEmitter.h"

#include "ParticleData.h"
#include "PositionGenerator.h"
#include "VelocityGenerator.h"
#include "RadianGenerator.h"
#include "ColorGenerator.h"
#include "LifetimeGenerator.h"
#include "OriginGenerator.h"


ParticleEmitter::ParticleEmitter(float2& position, int emitRate)
	: position_{ position }
	, emitRate_{ emitRate }
{
	generators_[0] = new PositionGenerator{ position_ };
	generators_[1] = new VelocityGenerator{};
	generators_[2] = new RadianGenerator{};
	generators_[3] = new ColorGenerator{color_};
	generators_[4] = new LifetimeGenerator{};
	generators_[5] = new OriginGenerator{ position_ };
}


ParticleEmitter::~ParticleEmitter()
{
	for (ParticleGenerator* generator : generators_)
	{
		delete generator;
	}
}


void ParticleEmitter::Emit(float deltaTime, ParticleData* pData)
{
	emissionBuildup_ += deltaTime * emitRate_;

	if (emissionBuildup_ < 1.0f)
	{
		return;
	}
		
	const size_t maxNewParticles{ static_cast<size_t>(emissionBuildup_) };
	emissionBuildup_ -= maxNewParticles;

	// Start at the current empty position (that we are about to fill).
	const size_t startId{ pData->countAlive_ };

	// Where to stop (usually where we started + the number of particles we made).
	const size_t endId{ min(startId + maxNewParticles, pData->count_) };

	// Generate the new particles.
	for (ParticleGenerator* generator : generators_)
	{
		generator->Generate(deltaTime, pData, startId, endId);
	}

	// Wake new particles up.
	for (size_t index{ startId }; index < endId; ++index)
	{
		pData->Wake(index);
	}
}


void ParticleEmitter::ChangeColor(const uint color)
{
	color_ = color;
}