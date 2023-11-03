// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "ArcherList.h"

#include "GameObject.h"
#include "Archer.h"
#include "Vector2.h"

#include "ArcherRewindData.h"



ArcherList::~ArcherList()
{
	delete[] archers_;
}


void ArcherList::Initialize(const int size)
{
	capacity_ = size;
	archers_ = new Archer[capacity_];
}


void ArcherList::Create(const float2 startTileToCenterOn, Sprite* lockonSprite, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool)
{
	if (currentPoolIndex_ < capacity_)
	{
		archers_[currentPoolIndex_++].Init(startTileToCenterOn, lockonSprite, scorekeeper, playerPosition, arrowPool);
	}
}


void ArcherList::Reset()
{
	capacity_ = 0;
	currentPoolIndex_ = 0;

	delete[] archers_;
	archers_ = nullptr;
}


void ArcherList::Update(float deltaTime, float gravity)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (archers_[index].isActive_)
		{
			archers_[index].Update(deltaTime, gravity);
		}
	}
}


void ArcherList::StoreRewindData(ArcherRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		archers_[index].StoreRewindData(rewindData[index]);
	}
}


void ArcherList::Rewind(ArcherRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		archers_[index].Rewind(rewindData[index]);
	}
}


void ArcherList::Resume(ArcherRewindData* rewindData)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{			
		archers_[index].Resume(rewindData[index]);
		
	}
}


void ArcherList::AddActive(GameObject* goList[], int& currentIndex)
{
	for (int index{ 0 }; index < currentPoolIndex_; ++index)
	{
		if (archers_[index].isActive_)
		{
			goList[currentIndex++] = &archers_[index];
		}
	}
}


const int ArcherList::GetObjectId() const
{
	return objectId_;
}


const int ArcherList::Size() const
{
	return currentPoolIndex_;
}