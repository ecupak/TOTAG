// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "RockPileList.h"

#include "GameObject.h"
#include "RockPile.h"

#include "RockPileRewindData.h"


RockPileList::~RockPileList()
{
	delete[] rockPiles_;
}


void RockPileList::Initialize(const int size)
{
	capacity_ = size;
	rockPiles_ = new RockPile[capacity_];
}


RockPile* RockPileList::Create(const float2 startTileToCenterOn, Sprite* sprite)
{
	if (currentPoolIndex_ < capacity_)
	{
		rockPiles_[currentPoolIndex_++].Init(startTileToCenterOn, sprite);
		return &rockPiles_[currentPoolIndex_ - 1];
	}

	return nullptr;
}


void RockPileList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] rockPiles_;
	rockPiles_ = nullptr;
}


void RockPileList::StoreRewindData(RockPileRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		rockPiles_[index].StoreRewindData(rewindData[index]);
	}
}


void RockPileList::Rewind(RockPileRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		rockPiles_[index].Rewind(rewindData[index]);
	}
}


void RockPileList::Resume(RockPileRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		rockPiles_[index].Resume(rewindData[index]);
	}
}


void RockPileList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (rockPiles_[index].isActive_)
		{
			goList[currentIndex++] = &rockPiles_[index];
		}
	}
}


const int RockPileList::GetObjectId() const
{
	return objectId_;
}