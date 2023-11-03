// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.

#include "precomp.h"
#include "DynamicParticleSystem.h"

#include "ParticleData.h"
#include "ParticleRenderer.h"
#include "ParticleEmitter.h"
#include "ParticleGenerator.h"
#include "PositionUpdater.h"
#include "RadianUpdater.h"
#include "ColorUpdater.h"
#include "LifetimeUpdater.h"
#include "ParticleTrasher.h"


DynamicParticleSystem::DynamicParticleSystem(float2& position, size_t maxParticleCount, int emitRate)
	: position_{ position }
	, maxParticleCount_{ maxParticleCount }
{
	maxParticleCount_ = maxParticleCount;

	// Add data
	pData_ = new ParticleData{ maxParticleCount };

	// Add renderer
	pRenderer_ = new ParticleRenderer{};

	// Add emitter
	pEmitter_ = new ParticleEmitter{ position_, emitRate };

	// Add updaters
	pUpdaters_[0] = new PositionUpdater{ position_ };
	pUpdaters_[1] = new RadianUpdater{};
	pUpdaters_[2] = new ColorUpdater{};
	pUpdaters_[3] = new LifetimeUpdater{};

	// Add trasher.
	pTrasher_ = new ParticleTrasher{};
}


DynamicParticleSystem::~DynamicParticleSystem()
{
	delete pData_;
	delete pRenderer_;
	delete pEmitter_;
	delete pTrasher_;

	for (int index{ 0 }; index < 4; ++index)
	{
		delete pUpdaters_[index];
	}
}


void DynamicParticleSystem::Update(float deltaTime)
{
	if (isActive_)
	{
		Emit(deltaTime);
		RunUpdaters(deltaTime);
		Cleanup();
	}
}


void DynamicParticleSystem::Emit(float deltaTime)
{
	pEmitter_->Emit(deltaTime, pData_);
}


void DynamicParticleSystem::RunUpdaters(float deltaTime)
{
	for (ParticleUpdater* updater : pUpdaters_)
	{
		updater->Update(deltaTime, pData_);
	}
}


void DynamicParticleSystem::Cleanup()
{
	pTrasher_->Cleanup(pData_);
}


void DynamicParticleSystem::Render(Surface* screen, int xOffset, int yOffset)
{
	if (isActive_)
	{
		pRenderer_->Draw(screen, xOffset, yOffset, pData_, 0);
	}
}