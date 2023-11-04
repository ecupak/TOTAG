#include "precomp.h"
#include "ParticleSystem.h"

#include "ParticleData.h"
#include "ParticleRenderer.h"
#include "ParticleEmitter.h"
#include "ParticleGenerator.h"
#include "PositionUpdater.h"
#include "RadianUpdater.h"
#include "ColorUpdater.h"
#include "LifetimeUpdater.h"
#include "ParticleTrasher.h"

#include "ParticleRewindData.h"

#include "Event.h"


// [Credit] https://www.cppstories.com/2014/04/flexible-particle-system-start/
// All parts of the particle system were heavily inspired by, if not lifted entirely from, this guide.
ParticleSystem::ParticleSystem() {}


ParticleSystem::ParticleSystem(float2 position, size_t maxParticleCount, int emitRate)
{
	Initialize(position, maxParticleCount, emitRate);
}


void ParticleSystem::Initialize(float2 position, size_t maxParticleCount, int emitRate)
{
	position_ = float2{ position.x + 8.0f, position.y };
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
	pUpdaters_[2] = new LifetimeUpdater{};

	// Add trasher.
	pTrasher_ = new ParticleTrasher{};

	isActive_ = true;
}


ParticleSystem::~ParticleSystem()
{
	delete pData_;
	delete pRenderer_;
	delete pEmitter_;
	delete pTrasher_;

	for (ParticleUpdater* updater : pUpdaters_)
	{
		delete updater;
	}
}


void ParticleSystem::Update(float deltaTime)
{
	if (isActive_)
	{
		Emit(deltaTime);
		RunUpdaters(deltaTime);
		Cleanup();
	}
}


void ParticleSystem::Emit(float deltaTime)
{
	pEmitter_->Emit(deltaTime, pData_);
}


void ParticleSystem::RunUpdaters(float deltaTime)
{
	for (ParticleUpdater* updater : pUpdaters_)
	{
		updater->Update(deltaTime, pData_);
	}
}


void ParticleSystem::Cleanup()
{
	pTrasher_->Cleanup(pData_);
}


void ParticleSystem::Render(Surface* screen, int xOffset, int yOffset)
{
	if (isActive_)
	{
		pRenderer_->Draw(screen, static_cast<int>(position_.x) - xOffset, static_cast<int>(position_.y) - yOffset, pData_, color_);
	}
}


void ParticleSystem::OnNotify(const ObjectClass, const Event event)
{
	if (event == Event::OnTouch)
	{
		color_ = 0x00FF00;
	}
}


void ParticleSystem::StoreRewindData(ParticleRewindData& rewindData)
{
	rewindData.countAlive_ = pData_->countAlive_;
	rewindData.color_ = color_;

	size_t particleCount{ pData_->count_ };

	for (int index{ 0 }; index < particleCount; ++index)
	{
		rewindData.positions_[index] = pData_->positions_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		rewindData.radians_[index] = pData_->radians_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		rewindData.lifetimes_[index] = pData_->lifetimes_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		rewindData.isAlives_[index] = pData_->isAlives_[index];
	}
}


void ParticleSystem::Rewind(ParticleRewindData& rewindData)
{
	pData_->countAlive_ = rewindData.countAlive_;
	color_ = rewindData.color_;

	size_t particleCount{ pData_->count_ };

	for (int index{ 0 }; index < particleCount; ++index)
	{
		pData_->positions_[index] = rewindData.positions_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		pData_->radians_[index] = rewindData.radians_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		pData_->lifetimes_[index] = rewindData.lifetimes_[index];
	}

	for (int index{ 0 }; index < particleCount; ++index)
	{
		pData_->isAlives_[index] = rewindData.isAlives_[index];
	}
}


void ParticleSystem::Resume(ParticleRewindData&) {}