// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "ArrowPool.h"

#include "GameObject.h"
#include "Arrow.h"

#include "ArrowRewindData.h"


ArrowPool::ArrowPool(const int poolSize)
	: poolSize_{ poolSize }
	, arrows_{ new Arrow[poolSize_]{} }
{	}


ArrowPool::~ArrowPool()
{
	delete[] arrows_;
}


Arrow* ArrowPool::Create(const float2& position, const int archerWidth, const float2& direction, const float speed)
{
	// Find available spot.
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (!arrows_[index].isActive_)
		{
			arrows_[index].Init(position, archerWidth, direction, speed);
			return &arrows_[index];
		}
	}

	// No free spot available.
	return nullptr;
}


void ArrowPool::Reset()
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		arrows_[index].isActive_ = false;
	}
}


void ArrowPool::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (arrows_[index].isActive_)
		{
			arrows_[index].Update(deltaTime, gravity);
		}
	}
}


void ArrowPool::StoreRewindData(ArrowRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		arrows_[index].StoreRewindData(rewindData[index]);
	}
}


void ArrowPool::Rewind(ArrowRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		arrows_[index].Rewind(rewindData[index]);
	}
}


void ArrowPool::Resume(ArrowRewindData* rewindData)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		arrows_[index].Resume(rewindData[index]);
	}
}


void ArrowPool::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (arrows_[index].isActive_)
		{
			goList[currentIndex++] = &arrows_[index];
		}
	}
}