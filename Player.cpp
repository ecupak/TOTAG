#include "precomp.h"
#include "Player.h"

#include "Shape.h"

#include "KeyboardManager.h"
#include "Level.h"
#include "Hitbox.h"
#include "TilesetData.h"
#include "MapData.h"
#include "TileCollision.h"

// All states so we can switch to the correct one after rewinding.
#include "PlayerState.h"
#include "PlayerBouncingState.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerClimbingState.h"
#include "PlayerZipliningState.h"
#include "PlayerRecoilingState.h"
#include "PlayerSwingingState.h"

#include "PlayerCommand.h"

#include "PlayerRewindData.h"

#include <iostream>


Player::Player(RockPool* rockPool)
	: Actor{ ObjectClass::Player, 0, 0, ObjectId::Player, ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Collectible | ObjectId::Trigger }
	, rockPool_{ rockPool }
{
	// Activate gameobject;
	isActive_ = true;

	// Shape is the world position of the object's sprite. Use the top-left corner as start.
	shape_->size_ = float2{ TILE_WIDTH, TILE_HEIGHT };

	// Hitboxes (head, body, (core), tail, feet).
	hitboxCount_ = HitboxRole::HitboxRoleCount;
	hitboxes_ = new Hitbox[hitboxCount_];

	hitboxes_[0] = Hitbox{ shape_->size_.x };	
	hitboxes_[1] = Hitbox{ shape_->size_.x };
	hitboxes_[2] = Hitbox{ shape_->size_.x };
	hitboxes_[3] = Hitbox{ shape_->size_.x };
	hitboxes_[4] = Hitbox{ shape_->size_.x };

	// The various sprites of the states we can be in. Preload them and keep them ready to swap out on the fly.
	spriteSheetsCount_ = StateName::StateNameCount;
	spriteSheets_ = new Sprite* [spriteSheetsCount_];

	spriteSheets_[StateName::Standing] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Walking] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Jumping] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Falling] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Climbing] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Bouncing] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Ziplining] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Recoiling] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };
	spriteSheets_[StateName::Swinging] = new Sprite{ new Surface{"assets/sprites/player_walk.png"}, true, TILE_WIDTH, TILE_HEIGHT };

	// Player walking speed.
	horizontalAcceleration_ = 300.0f;

	// Set starting state.
	state_ = new PlayerStandingState{ *this };
	state_->Enter();	
}


Player::~Player() {}


void Player::Place(const float2 startingPosition)
{
	shape_->SetPosition(startingPosition);
}


void Player::Reset()
{
	// Restore values for start of level.
	currentHp_ = maxHp_;
	rockInventory_ = 3;
	isActive_ = true;

	canGrabLadder_ = true;
	canGrabRope_ = true;
	canGrabZipline_ = true;
	canThrow_ = true;
	isInvulnerable_ = true;

	ladderDelay_ = 0.0f;
	ropeDelay_ = 0.0f;
	ziplineDelay_ = 0.0f;
	throwDelay_ = 0.0f;
	elapsedInvulnerableTime_ = maxInvulnerableTime_;

	isAtLevelEnd_ = false;

	*velocity_ = float2{ 0.0f, 0.0f };

	// Change to starting state.
	PlayerState* state{ new PlayerStandingState{ *this } };
	ChangeState(state);
}


void Player::Update(float deltaTime, float gravity, KeyboardManager& km)
{
	// Do nothing if not onstage.
	if (!isOnstage_)
	{
		return;
	}

	// Run state with player-generated input.
	command_ = nullptr;
	ActorState* state{ state_->HandleInput(km, command_) };

	if (state != nullptr)
	{
		ChangeState(state);
	}

	// Run command, update velocity.
	FinishUpdate(deltaTime, gravity);
}


void Player::StoreRewindData(PlayerRewindData* rewindData)
{
	if (isActive_)
	{
		rewindData->canGrabLadder_ = canGrabLadder_;
		rewindData->canGrabRope_ = canGrabRope_;
		rewindData->canGrabZipline_ = canGrabZipline_;
		rewindData->canThrow_ = canThrow_;
		rewindData->isInvulnerable_ = isInvulnerable_;

		rewindData->ladderDelay_ = ladderDelay_;
		rewindData->ropeDelay_ = ropeDelay_;
		rewindData->ziplineDelay_ = ziplineDelay_;
		rewindData->throwDelay_ = throwDelay_;
		rewindData->elapsedInvulnerableTime_ = elapsedInvulnerableTime_;

		rewindData->rockInventory_ = rockInventory_;
		rewindData->stateIndex_ = state_->GetStateIndex();

		state_->StoreRewindData(rewindData->playerStateRewindData_);
	}

	Actor::StoreRewindData(rewindData->actorRewindData_);
}


void Player::Rewind(PlayerRewindData* rewindData)
{
	canGrabLadder_ = rewindData->canGrabLadder_;
	canGrabRope_ = rewindData->canGrabRope_;
	canGrabZipline_ = rewindData->canGrabZipline_;
	canThrow_ = rewindData->canThrow_;
	isInvulnerable_ = rewindData->isInvulnerable_;

	ladderDelay_ = rewindData->ladderDelay_;
	ropeDelay_ = rewindData->ropeDelay_;
	ziplineDelay_ = rewindData->ziplineDelay_;
	throwDelay_ = rewindData->throwDelay_;
	elapsedInvulnerableTime_ = rewindData->elapsedInvulnerableTime_;

	rockInventory_ = rewindData->rockInventory_;

	sprite_ = spriteSheets_[rewindData->stateIndex_];

	Actor::Rewind(rewindData->actorRewindData_);

	SetSpriteFacing(isFlipped_);
	isSpriteBlinking_ = false;
}


void Player::Resume(PlayerRewindData* rewindData)
{
	ActorState* state{ GetResumedState(rewindData)};

	if (state != nullptr)
	{
		ChangeState(state, true);
	}

	Actor::Resume(rewindData->actorRewindData_);
}


ActorState* Player::GetResumedState(PlayerRewindData* rewindData)
{
	switch (rewindData->stateIndex_)
	{
	case Player::StateName::Bouncing:
		return new PlayerBouncingState{ *this };
	case Player::StateName::Climbing:
		return new PlayerClimbingState{ *this, rewindData->playerStateRewindData_ };
	case Player::StateName::Falling:
		return new PlayerFallingState{ *this };
	case Player::StateName::Jumping:
		return new PlayerJumpingState{ *this, rewindData->playerStateRewindData_ };
	case Player::StateName::Recoiling:
		return new PlayerRecoilingState{ *this, rewindData->playerStateRewindData_ };
	case Player::StateName::Standing:
		return new PlayerStandingState{ *this };
	case Player::StateName::Swinging:
		return new PlayerSwingingState{ *this, rewindData->playerStateRewindData_ };
	case Player::StateName::Walking:
		return new PlayerWalkingState{ *this, rewindData->playerStateRewindData_ };
	case Player::StateName::Ziplining:
		return new PlayerZipliningState{ *this, rewindData->playerStateRewindData_ };
	default:
		return nullptr;
	}
}


// Resolution after movement.
void Player::ResolveObstacleCollisions(const float2& contactNormals)
{
	Actor::ResolveObstacleCollisions(contactNormals);

	// Hit wall.
	isAgainstWall_ = (contactNormals.x != 0.0f);
	if (isAgainstWall_ && state_->stateIndex_ != Player::StateName::Recoiling)
	{
		velocity_->x = 0.0;
	}

	// Resolve damage.
	if (AreHitboxesTouchingInteractTile(0b0110, TileCollisionID::Spike) && WasDamaged(1))
	{
		ChangeState(new PlayerRecoilingState{ *this, contactNormals });
	}
}


// Notification that object collision are done being processed.
void Player::ResolveObjectCollisions()
{
	// Check damage -> recoil first, then any interactions.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		// Touched enemy body - recoil and damage.
		if (objectCollisionList_[index]->objectLayer_ & (ObjectId::Enemy | ObjectId::EnemyAttack) && WasDamaged(1))
		{
			// If enemy is right of avatar, recoil to the left and vice versa.
			float horizontalRecoil{ objectCollisionList_[index]->shape_->center_.x > shape_->center_.x ? -1.0f : 1.0f };
			ChangeState(new PlayerRecoilingState{ *this, { horizontalRecoil, -1.0f} });
		}
	}

	// Check for swinging rope.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		if (objectCollisionList_[index]->objectClass_ == ObjectClass::Rope && canGrabRope_)
		{
			ChangeState(new PlayerSwingingState{ *this, objectCollisionList_[index] });
		}
	}

	// Pick up rock pile.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		if (objectCollisionList_[index]->objectClass_ == ObjectClass::RockPile)
		{
			rockInventory_ += 3;
		}
	}

	// Check for level end.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		if (objectCollisionList_[index]->objectClass_ == ObjectClass::Exit)
		{
			isAtLevelEnd_ = true;
		}
	}

	// Reset list.
	Actor::ResolveObjectCollisions();
}


const bool Player::WasDamaged(const int damageAmount)
{
	if (isInvulnerable_)
	{
		return false;
	}
	else
	{
		Actor::TakeDamage(damageAmount);

		// if dead, do something.

		return true;
	}
}


// Timers that handle states that are universal to all states.
void Player::ProcessTimers(float deltaTime)
{
	if (!canGrabLadder_)
	{
		ladderDelay_ -= deltaTime;

		if (ladderDelay_ <= 0.0f)
		{
			canGrabLadder_ = true;
		}
	}

	if (!canGrabZipline_)
	{
		ziplineDelay_ -= deltaTime;

		if (ziplineDelay_ <= 0.0f)
		{
			canGrabZipline_ = true;
		}
	}

	if (!canGrabRope_)
	{
		ropeDelay_ -= deltaTime;

		if (ropeDelay_ <= 0.0f)
		{
			canGrabRope_ = true;
		}
	}

	if (!canThrow_)
	{
		throwDelay_ -= deltaTime;

		if (throwDelay_ <= 0.0f)
		{
			canThrow_ = true;
		}
	}

	if (isInvulnerable_)
	{
		elapsedInvulnerableTime_ += deltaTime;
		isSpriteBlinking_ = !isSpriteBlinking_;

		if (elapsedInvulnerableTime_ >= maxInvulnerableTime_)
		{
			isSpriteBlinking_ = false;
			isInvulnerable_ = false;
			elapsedInvulnerableTime_ = 0.0f;
		}
	}
}


// If hitboxFlags is 0b11010, then the hitboxes at indexes [1, 3, 4] will be checked (read right to left).
void Player::AddTileMask(int hitboxFlags, const int tileFlags)
{
	for (int hitboxIndex{ 0 }; hitboxIndex < hitboxCount_; ++hitboxIndex)
	{
		if (hitboxFlags & (1 << hitboxIndex))
		{
			hitboxes_[hitboxIndex].tileMask_ |= tileFlags;
		}
	}

	GetAllTileMasks();
}


void Player::Draw(Surface* screen, int x, int y)
{
	if (!isSpriteBlinking_)
	{
		GameObject::Draw(screen, x, y);
	}

#ifdef _DEBUG
	screen->Print(state_->name_, x, y - 16, 0x00FFFF);

	screen->Box(x, y, x + static_cast<int>(shape_->size_.x - 1), static_cast<int>(y + shape_->size_.y - 1), 0xffffff);

	for (int i{ 0 }; i < Player::HitboxRole::HitboxRoleCount; ++i)
	{
		screen->Box(x + hitboxes_[i].offset_.x, y + hitboxes_[i].offset_.y, x + hitboxes_[i].offset_.x + hitboxes_[i].size_.x - 1, y + hitboxes_[i].offset_.y + hitboxes_[i].size_.y - 1, 0x00ffff);
	}
#endif // _DEBUG
}


const int Player::GetCurrentHP() const
{
	return currentHp_;
}


const int Player::GetRockInventory() const
{
	return rockInventory_;
}


const bool Player::IsAtLevelEnd() const
{
	return isAtLevelEnd_;
}