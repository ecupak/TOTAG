// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "SoulList.h"

#include "GameObject.h"
#include "Soul.h"

#include "SoulRewindData.h"


SoulList::~SoulList()
{
	delete[] souls_;
}


void SoulList::Initialize(const int size)
{
	capacity_ = size;
	souls_ = new Soul[capacity_];
}


Soul* SoulList::Create(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper)
{
	if (currentPoolIndex_ < capacity_)
	{
		souls_[currentPoolIndex_++].Init(startTileToCenterOn, sprite, scorekeeper);
		return &souls_[currentPoolIndex_ - 1];
	}

	return nullptr;
}


void SoulList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] souls_;
	souls_ = nullptr;
}


void SoulList::StoreRewindData(SoulRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		souls_[index].StoreRewindData(rewindData[index]);
	}
}


void SoulList::Rewind(SoulRewindData* rewindData, const bool isFromSaveFile)
{
	if (isFromSaveFile)
	{
		for (int index{ 0 }; index < currentPoolIndex_; ++index)
		{
			souls_[index].Rewind(rewindData[index]);
		}
	}
}


void SoulList::Resume(SoulRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		souls_[index].Resume(rewindData[index]);
	}
}


void SoulList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (souls_[index].isActive_)
		{
			goList[currentIndex++] = &souls_[index];
		}
	}
}


const int SoulList::GetObjectId() const
{
	return objectId_;
}