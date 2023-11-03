#include "precomp.h"
#include "RewindManager.h"

#include "GameObject.h"
#include "FrameData.h"

#include <iostream>


RewindManager::RewindManager() {}


RewindManager::~RewindManager()
{
	delete[] frameData_;
}


void RewindManager::SetRewindArraySize(ObjectClass objectClass, const int size)
{
	switch (objectClass)
	{
	case ObjectClass::Knight:
		knightCount_ = size;
		break;
	case ObjectClass::Archer:
		archerCount_ = size;
		break;
	case ObjectClass::Coin:
		coinCount_ = size;
		break;
	case ObjectClass::Soul:
		soulCount_ = size;
		break;
	case ObjectClass::RockPile:
		rockPileCount_ = size;
		break;
	case ObjectClass::Rock:
		rockCount_ = size;
		break;
	case ObjectClass::Arrow:
		arrowCount_ = size;
		break;
	case ObjectClass::Rope:
		ropeCount_ = size;
		break;
	case ObjectClass::SavePoint:
		savePointCount_ = size;
		break;
	case ObjectClass::Particles:
		particleCount_ = size;
		break;
	default:
		break;
	}
}


void RewindManager::CreateFrameData(const int fps)
{
	// Create new data.
	int rewindDurationInSeconds{ 2 };
	frameDataSize_ = fps * rewindDurationInSeconds;
	frameData_ = new FrameData[frameDataSize_];

	for (int frameIndex{ 0 }; frameIndex < frameDataSize_; ++frameIndex)
	{
		frameData_[frameIndex].Initialize(archerCount_, knightCount_, ropeCount_, 
			arrowCount_, coinCount_, soulCount_, rockCount_, rockPileCount_, 
			savePointCount_, particleCount_);
	}
}


void RewindManager::Reset()
{
	// Set tail, head, and forward lock to 0.
	tail_ = 0;
	head_ = 0;
	forwardLock_ = 0;

	// Erase frame data arrays so they can be remade.
	delete[] frameData_;
}


PlayerRewindData* RewindManager::GetPlayerRewindData()
{
	return frameData_[head_].playerData_;
}


ArcherRewindData* RewindManager::GetArcherRewindData()
{
	return frameData_[head_].archerData_;
}


KnightRewindData* RewindManager::GetKnightRewindData()
{
	return frameData_[head_].knightData_;
}


SwingingRopeRewindData* RewindManager::GetSwingingRopeRewindData()
{
	return frameData_[head_].ropeData_;
}


ArrowRewindData* RewindManager::GetArrowRewindData()
{
	return frameData_[head_].arrowData_;
}


CoinRewindData* RewindManager::GetCoinRewindData()
{
	return frameData_[head_].coinData_;
}


SoulRewindData* RewindManager::GetSoulRewindData()
{
	return frameData_[head_].soulData_;
}


RockRewindData* RewindManager::GetRockRewindData()
{
	return frameData_[head_].rockData_;
}


RockPileRewindData* RewindManager::GetRockPileRewindData()
{
	return frameData_[head_].rockPileData_;
}


SavePointRewindData* RewindManager::GetSavePointRewindData()
{
	return frameData_[head_].savePointData_;
}


ParticleRewindData* RewindManager::GetParticleRewindData()
{
	return frameData_[head_].particleData_;
}


CameraRewindData* RewindManager::GetCameraRewindData()
{
	return frameData_[head_].cameraData_;
}


GameRewindData* RewindManager::GetGameRewindData()
{
	return frameData_[head_].gameData_;
}


const int RewindManager::GetKnightCount() const
{ 
	return knightCount_; 
}


const int RewindManager::GetArcherCount() const
{ 
	return archerCount_; 
}


const int RewindManager::GetCoinCount() const
{ 
	return coinCount_; 
}


const int RewindManager::GetSoulCount() const
{
	return soulCount_;
}


const int RewindManager::GetRockPileCount() const
{
	return rockPileCount_;
}


const int RewindManager::GetSavePointCount() const
{
	return savePointCount_;
}


const int RewindManager::GetParticleCount() const
{
	return particleCount_;
}


const int RewindManager::GetRockCount() const
{
	return rockCount_;
}


const int RewindManager::GetRopeCount() const
{
	return ropeCount_;
}


const int RewindManager::GetArrowCount() const
{
	return arrowCount_;
}


// Head advancement during "normal time".
void RewindManager::AdvanceHead()
{
	head_ = ++head_ % frameDataSize_;
	forwardLock_ = head_;

	// Push tail ahead if head has reached it.
	if (head_ == tail_)
	{
		tail_ = ++tail_ % frameDataSize_;
	}
}


// Locked head is the max head position from "normal time". 
// At the start of the game, the rewind frame advances once before any data is written.
// To prevent the player from rewinding into this dead zone, can only rewind 1 before tail.
// While controlling the rewind position, player must stay within [tail + 1 ... lockedHead_].
void RewindManager::ControlHead(const int rewindDirection)
{
	if (rewindDirection == -1)
	{
		if (head_ == tail_ + 1)
		{
			return;
		}

		if (--head_ < 0)
		{
			head_ = frameDataSize_ - 1;
		}
	}
	else
	{
		if (head_ == forwardLock_)
		{
			return;
		}

		if (++head_ > frameDataSize_ - 1)
		{
			head_ = 0;
		}
	}
}


// Player has rewinded and resumed at a past point. Lockedhead should now be at head.
void RewindManager::SetForwardLockToHead()
{
	forwardLock_ = head_;
}