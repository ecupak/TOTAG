// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "CoinList.h"

#include "GameObject.h"
#include "Coin.h"

#include "CoinRewindData.h"


CoinList::~CoinList()
{
	delete[] coins_;
}


void CoinList::Initialize(const int size)
{
	capacity_ = size;
	coins_ = new Coin[capacity_];
}


Coin* CoinList::Create(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper)
{
	if (currentPoolIndex_ < capacity_)
	{
		coins_[currentPoolIndex_++].Init(startTileToCenterOn, sprite, scorekeeper);
		return &coins_[currentPoolIndex_ - 1];
	}

	return nullptr;
}


void CoinList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] coins_;
	coins_ = nullptr;
}


void CoinList::StoreRewindData(CoinRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		coins_[index].StoreRewindData(rewindData[index]);
	}
}


void CoinList::Rewind(CoinRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		coins_[index].Rewind(rewindData[index]);
	}
}


void CoinList::Resume(CoinRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		coins_[index].Resume(rewindData[index]);
	}
}


void CoinList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (coins_[index].isActive_)
		{
			goList[currentIndex++] = &coins_[index];
		}
	}
}


const int CoinList::GetObjectId() const
{
	return objectId_;
}