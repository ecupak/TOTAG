// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "KnightPool.h"

#include "GameObject.h"
#include "Knight.h"
#include "Vector2.h"


KnightPool::KnightPool(const int poolSize, const Vector2* playerPosition)
	: poolSize_{ poolSize }
	, playerPosition_{ playerPosition }
	, knights_{ new Knight[poolSize_] }
{	}


KnightPool::~KnightPool()
{
	delete[] knights_;
}


Knight* KnightPool::Create(float2 knightPosition)
{
	// Find available spot.
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (!knights_[index].isActive_)
		{
			knights_[index].Init(knightPosition, playerPosition_);
			return &knights_[index];
		}
	}

	// No free spot available.
	return nullptr;
}


void KnightPool::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (knights_[index].isActive_)
		{
			knights_[index].Update(deltaTime, gravity);
		}
	}
}


void KnightPool::AddActive(GameObject* goList[], int& currentIndex)
{
	activeCount_ = 0;

	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (knights_[index].isActive_)
		{
			goList[currentIndex++] = &knights_[index];
			++activeCount_;
		}
	}
}