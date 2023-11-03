// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "KnightList.h"

#include "GameObject.h"
#include "Knight.h"
#include "Vector2.h"

#include "KnightRewindData.h"


KnightList::~KnightList()
{
	delete[] knights_;
}


void KnightList::Initialize(const int size)
{
	capacity_ = size;
	knights_ = new Knight[capacity_];
}


void KnightList::Create(const float2 startTileToCenterOn, Scorekeeper* scorekeeper, const Vector2* playerPosition)
{
	if (currentPoolIndex_ < capacity_)
	{
		knights_[currentPoolIndex_++].Init(startTileToCenterOn, scorekeeper, playerPosition);
	}
}


void KnightList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] knights_;
	knights_ = nullptr;
}


void KnightList::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (knights_[index].isActive_)
		{
			knights_[index].Update(deltaTime, gravity);
		}
	}
}


void KnightList::StoreRewindData(KnightRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		knights_[index].StoreRewindData(rewindData[index]);
	}
}


void KnightList::Rewind(KnightRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		knights_[index].Rewind(rewindData[index]);
	}
}


void KnightList::Resume(KnightRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		knights_[index].Resume(rewindData[index]);
	}
}


void KnightList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (knights_[index].isActive_)
		{
			goList[currentIndex++] = &knights_[index];
		}
	}
}


const int KnightList::GetObjectId() const
{
	return objectId_;
}