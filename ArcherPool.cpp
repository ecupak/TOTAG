// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "ArcherPool.h"

#include "GameObject.h"
#include "Archer.h"
#include "Vector2.h"


ArcherPool::ArcherPool(const Vector2* playerPosition, ArrowPool* arrowPool)
	: playerPosition_{playerPosition}
	, arrowPool_{ arrowPool }
{	}


ArcherPool::~ArcherPool()
{
	delete[] archers_;
}


void ArcherPool::Reserve(const int poolSize)
{
	poolSize_ = poolSize;
	archers_ = new Archer[poolSize];
}


Archer* ArcherPool::Create(float2 archerPosition)
{
	// Find available spot.
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (!archers_[index].isActive_)
		{
			archers_[index].Init(archerPosition, playerPosition_, arrowPool_);
			return &archers_[index];
		}
	}

	// No free spot available.
	return nullptr;
}


void ArcherPool::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (archers_[index].isActive_)
		{
			archers_[index].Update(deltaTime, gravity);
		}
	}
}


void ArcherPool::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < poolSize_; ++index)
	{
		if (archers_[index].isActive_)
		{
			goList[currentIndex++] = &archers_[index];
		}
	}
}


const int ArcherPool::GetObjectId() const
{
	return objectId_;
}