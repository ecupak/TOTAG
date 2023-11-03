#include "precomp.h"
#include "Actor.h"

#include "Shape.h"

#include "KeyboardManager.h"
#include "Level.h"
#include "Hitbox.h"
#include "TilesetData.h"
#include "MapData.h"
#include "TileCollision.h"

#include "ActorState.h"
#include "Command.h"

#include "ActorRewindData.h"


Actor::Actor(ObjectClass objectClass, int tileLayer, int tileMask, int objectLayer, int objectMask)
	: GameObject{ objectClass, ObjectType::Actor, tileLayer, tileMask, objectLayer, objectMask }
{	}


Actor::~Actor()
{
	delete state_;

	for (int index{ 0 }; index < spriteSheetsCount_; ++index)
	{
		delete spriteSheets_[index];
	}

	delete[] spriteSheets_;
}


// Change to new state.
void Actor::ChangeState(ActorState* newState, const bool isFromRewind)
{
	// Exit current state.
	state_->Exit();

	// Swap states.
	delete state_;
	state_ = newState;

	// Do any enter actions of the new state.
	state_->Enter(isFromRewind);
}


// Delegate tasks to the current state class. Always update the collision box regardless of state.
void Actor::FinishUpdate(float deltaTime, float gravity)
{
	// Advance and resolve completed timers.
	ProcessTimers(deltaTime);

	// Apply downward acceleration.
	velocity_->y += gravity * gravityModifier_ * deltaTime;

	// Execute command from state input. Usually affects velocity.
	if (command_ != nullptr)
	{
		command_->Execute(deltaTime);
		delete command_;
	}

	// Do any extra steps of the current state. This may overwrite velocity - must do after gravity and command.
	state_->Update(deltaTime);

	// Set frameVelocity for collision calculations. This is the amount to apply to position.
	*frameVelocity_ = *velocity_ * deltaTime;

	// Slow down horizontal movement for next frame. Do at end so player can experience max velocity.
	ApplyFriction(deltaTime);
}


// Resolution after movement.
void Actor::ResolveObstacleCollisions(const float2& contactNormals)
{
	//frameVelocity_->y = frameVelocity_->y == 0.0f ? 0.0f : frameVelocity_->y;

	shape_->MovePosition(static_cast<float2>(*frameVelocity_));
	shape_->Truncate(static_cast<float2>(contactNormals));

	// Hit floor.
	isOnGround_ = (contactNormals.y == -1.0f);
	if (isOnGround_)
	{
		velocity_->y = 0.0;
	}

	// Hit ceiling.
	if (contactNormals.y == 1.0f)
	{
		velocity_->y = 0.0;
	}
}


// Notification of collision with reference to other.
void Actor::OnObjectCollision(GameObject* object)
{
	if (objectCollisionCount_ < 5)
	{
		objectCollisionList_[objectCollisionCount_] = object;
		++objectCollisionCount_;
	}
}


// Notification that object collision are done being processed. 
void Actor::ResolveObjectCollisions()
{
	// "Reset" object collision list.
	objectCollisionCount_ = 0;
}


void Actor::TakeDamage(const int damageAmount)
{
	currentHp_ -= damageAmount;
}


// Timers that handle states that are universal to all states.
void Actor::ProcessTimers(float) {}


// Takes all tile mask flags from current hitboxes and combines them into the actor's tile mask.
void Actor::GetAllTileMasks()
{
	tileMask_ = TileCollisionID::Nothing;

	for (int index{ 0 }; index < hitboxCount_; ++index)
	{
		tileMask_ |= hitboxes_[index].tileMask_;
	}
}


// This series of methods "Are/IsHitbox(es)TouchingTile" are used by player states to determine if they can move to another state.
// Hitboxes only store collisions with interacts. overlappingTileIds_ store overlaps with all obstacles and interacts.
// If a newly entered state needs to know if it overlaps something (like the Falling state), use variable overlappingTileIds_ instead.
 
// Check multiple hitboxes for overlapping tile. Stop on first confirmed.
// If hitboxFlags is 0b11010, then the hitboxes at indexes [1, 3, 4] will be checked (read right to left).
const bool Actor::AreHitboxesTouchingInteractTile(const int hitboxFlags, const int tileFlags) const
{
	for (int hitboxIndex{ 0 }; hitboxIndex < hitboxCount_; ++hitboxIndex)
	{
		if (hitboxFlags & (1 << hitboxIndex))
		{
			if (IsHitboxTouchingInteractTile(hitboxIndex, tileFlags))
			{
				return true;
			}
		}
	}

	return false;
}


const bool Actor::IsHitboxTouchingInteractTile(const int hitboxIndex, const int tileFlags) const
{
	for (int index{ 0 }; index < 4; ++index)
	{
		const MapTile* mapTile{ hitboxes_[hitboxIndex].overlappingTiles_[index] };

		if (mapTile == nullptr || mapTile->interactTile_ == nullptr)
		{
			break;
		}

		if (mapTile->interactTile_->hitboxes_[0]->tileLayer_ & tileFlags)
		{
			return true;
		}
	}

	return false;
}


const bool Actor::IsHitboxTouchingInteractTile(const int hitboxIndex, const int tileFlags, const MapTile*& outTile) const
{
	for (int index{ 0 }; index < 4; ++index)
	{
		const MapTile* mapTile{ hitboxes_[hitboxIndex].overlappingTiles_[index] };

		if (mapTile == nullptr || mapTile->interactTile_ == nullptr)
		{
			break;
		}

		if (mapTile->interactTile_->hitboxes_[0]->tileLayer_ & tileFlags)
		{
			outTile = mapTile;

			return true;
		}
	}

	return false;
}



const bool Actor::AreHitboxesTouchingMapTile(const int hitboxFlags, const int tileFlags) const
{
	for (int hitboxIndex{ 0 }; hitboxIndex < hitboxCount_; ++hitboxIndex)
	{
		if (hitboxFlags & (1 << hitboxIndex))
		{
			if (IsHitboxTouchingMapTile(hitboxIndex, tileFlags))
			{
				return true;
			}
		}
	}

	return false;
}


const bool Actor::IsHitboxTouchingMapTile(const int hitboxIndex, const int tileFlags) const
{
	for (int index{ 0 }; index < 4; ++index)
	{
		const MapTile* mapTile{ hitboxes_[hitboxIndex].overlappingTiles_[index] };

		if (mapTile == nullptr)
		{
			break;
		}

		if (mapTile->combinedCollisionLayer_ & tileFlags)
		{
			return true;
		}
	}

	return false;
}


void Actor::StoreRewindData(ActorRewindData* rewindData)
{
	if (isActive_)
	{
		rewindData->currentHp_ = currentHp_;
		rewindData->isAgainstWall_ = isAgainstWall_;
		rewindData->isOnGround_ = isOnGround_;
	}

	GameObject::StoreRewindData(rewindData->gameObjectRewindData_);
}


void Actor::Rewind(ActorRewindData* rewindData)
{
	currentHp_ = rewindData->currentHp_;

	GameObject::Rewind(rewindData->gameObjectRewindData_);
}


void Actor::Resume(ActorRewindData* rewindData)
{
	isAgainstWall_ = rewindData->isAgainstWall_;
	isOnGround_ = rewindData->isOnGround_;

	GameObject::Resume(rewindData->gameObjectRewindData_);
}

void Actor::SetSprite(const int spriteSheetId)
{
	sprite_ = spriteSheets_[spriteSheetId];
}


// Slip all sprite sheets and current hitboxes (hitboxes will initialize using isFlipped).
void Actor::SetSpriteFacing(bool isFlipped)
{
	for (int index{ 0 }; index < spriteSheetsCount_; ++index)
	{
		if (spriteSheets_[index] != nullptr)
		{
			spriteSheets_[index]->SetFacing(isFlipped);
		}
	}

	for (int hitboxIndex{ 0 }; hitboxIndex < hitboxCount_; ++hitboxIndex)
	{
		hitboxes_[hitboxIndex].Flip(isFlipped);
	}

	isFlipped_ = isFlipped;
}