// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "SwingingRopeList.h"

#include "GameObject.h"
#include "SwingingRope.h"
#include "Vector2.h"

#include "SwingingRopeRewindData.h"


SwingingRopeList::~SwingingRopeList()
{
	delete[] ropes_;
}


void SwingingRopeList::Initialize(const int size)
{
	capacity_ = size;
	ropes_ = new SwingingRope[capacity_];
}


SwingingRope* SwingingRopeList::Create(float2 startTileToCenterOn)
{
	if (currentPoolIndex_ < capacity_)
	{
		ropes_[currentPoolIndex_++].Init(startTileToCenterOn);
		return &ropes_[currentPoolIndex_ - 1];
	}

	return nullptr;
}


void SwingingRopeList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] ropes_;
	ropes_ = nullptr;
}


void SwingingRopeList::Update(float deltaTime)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		ropes_[index].Update(deltaTime);
	}
}


void SwingingRopeList::StoreRewindData(SwingingRopeRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		ropes_[index].StoreRewindData(rewindData[index]);
	}
}


void SwingingRopeList::Rewind(SwingingRopeRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		ropes_[index].Rewind(rewindData[index]);
	}
}


void SwingingRopeList::Resume(SwingingRopeRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		ropes_[index].Resume(rewindData[index]);
	}
}


void SwingingRopeList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (ropes_[index].isActive_)
		{
			goList[currentIndex++] = &ropes_[index];
		}
	}
}


const int SwingingRopeList::GetObjectId() const
{
	return objectId_;
}