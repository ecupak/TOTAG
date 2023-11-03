// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "RockPool.h"

#include "GameObject.h"
#include "Rock.h"

#include "RockRewindData.h"


RockPool::RockPool(const int poolSize)
	: poolSize_{ poolSize }
	, rocks_{ new Rock[poolSize_]{} }
{	}


RockPool::~RockPool()
{
	delete[] rocks_;
}


Rock* RockPool::Create(const float2 position, const float2 velocity)
{
	// Find available spot.
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (!rocks_[index].isActive_)
		{
			rocks_[index].Init(position, velocity, &rockSprite_);
			return &rocks_[index];
		}
	}

	// No free spot available.
	return nullptr;
}


void RockPool::Reset()
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		rocks_[index].isActive_ = false;
	}
}


void RockPool::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (rocks_[index].isActive_)
		{
			rocks_[index].Update(deltaTime, gravity);
		}
	}
}


void RockPool::StoreRewindData(RockRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		rocks_[index].StoreRewindData(rewindData[index]);
	}
}


void RockPool::Rewind(RockRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		rocks_[index].Rewind(rewindData[index]);
	}
}


void RockPool::Resume(RockRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		rocks_[index].Resume(rewindData[index]);
	}
}


void RockPool::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < poolSize_; ++index) 
	{
		if (rocks_[index].isActive_)
		{
			goList[currentIndex++] = &rocks_[index];
		}
	}
}