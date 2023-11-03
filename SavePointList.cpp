// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "SavePointList.h"

#include "GameObject.h"
#include "SavePoint.h"

#include "SavePointRewindData.h"


SavePointList::~SavePointList()
{
	delete[] savePoints_;
}


void SavePointList::Initialize(const int size)
{
	capacity_ = size;
	savePoints_ = new SavePoint[capacity_];
}


SavePoint* SavePointList::Create(const float2 startTileToCenterOn, Sprite* sprite)
{
	if (currentPoolIndex_ < capacity_)
	{
		savePoints_[currentPoolIndex_++].Init(startTileToCenterOn, sprite);
		return &savePoints_[currentPoolIndex_ - 1];
	}

	return nullptr;
}


void SavePointList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] savePoints_;
	savePoints_ = nullptr;
}


void SavePointList::Update(const float deltaTime) 
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		savePoints_[index].Update(deltaTime);
	}
}


void SavePointList::StoreRewindData(SavePointRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		savePoints_[index].StoreRewindData(rewindData[index]);
	}
}


void SavePointList::Rewind(SavePointRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		savePoints_[index].Rewind(rewindData[index]);
	}
}


void SavePointList::Resume(SavePointRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		savePoints_[index].Resume(rewindData[index]);
	}
}


void SavePointList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (savePoints_[index].isActive_)
		{
			goList[currentIndex++] = &savePoints_[index];
		}
	}
}


const int SavePointList::GetObjectId() const
{
	return objectId_;
}