#include "precomp.h"
#include "PlayerSwingingState.h"

#include "Player.h"
#include "Hitbox.h"
#include "GameObject.h"
#include "Shape.h"
#include "TileCollision.h"

#include "SwingingRope.h"

#include "PlayerJumpingState.h"

#include "LaunchCommand.h"

#include "KeyboardManager.h"
#include "Utility.h"
#include "Vector2.h"

#include "PlayerStateRewindData.h"


PlayerSwingingState::PlayerSwingingState(Player& player, GameObject* rope)
	: PlayerState{ player }
	, rope_{ dynamic_cast<SwingingRope*>(rope) }
{	}


PlayerSwingingState::PlayerSwingingState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, previousPosition_{ rewindData->vFloat2_ }
	, timeOnSegment_{ rewindData->vFloatA_ }
	, attachmentPoint_{ rewindData->vInt_ }
	, rope_{ rewindData->vRopePtr_ }
	, canJump_{ timeOnSegment_ > 1.0f }
{	}


void PlayerSwingingState::Enter(const bool isRewindEntry)
{
	name_ = "Swing";
	stateIndex_ = Player::StateName::Swinging;
	player_.SetSprite(stateIndex_);

	// Player no longer cares about rope while on it.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack;

	// Player has no tile collisions while swinging.
	SetHitboxData();

	player_.GetAllTileMasks();

	// There is no gravity or momentum while swinging.
	player_.gravityModifier_ = 0.0f;
	*player_.velocity_ = float2{ 0.0f, 0.0f };

	if (!isRewindEntry)
	{
		// Find initial attachment point.
		attachmentPoint_ = rope_->GetAttachmentPoint(static_cast<float2>(player_.shape_->center_));

		// Tell rope to go fast.
		rope_->SetHasPassenger(true);
	}
}


void PlayerSwingingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Ignore();

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Ignore();

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerSwingingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	if (canJump_ && km.IsJustPressed(Action::Jump))
	{
		// Find launch direction.
		float2 launchDirection{
			player_.shape_->start_.x - previousPosition_.x,
			player_.shape_->start_.y - previousPosition_.y
		};

		// Use adjusted values to get normalized vector for trajectory.
		float2 normalized{ normalize(launchDirection) };

		// Edit values to simulate certain scenarios.
		
		// ... If going downward (y is positive), set y to 0: cannot gain any height from releasing the rope while moving towards the ground.
		if (normalized.y > 0.0f)
		{
			normalized.y = 0.0f;
		}

		// ... Dampen the x value, since it's minimum is ~0.5 which gives a weird feeling.
		normalized.x *= normalized.x * (launchDirection.x >= 0 ? 1 : -1);
		
		// Launch!
 		command = new LaunchCommand{ &player_, float2{normalized * launchImpulse_} };

		player_.canGrabRope_ = false;
		player_.ropeDelay_ = 0.5f;
		
		return new PlayerJumpingState{ player_, true };
	}

	return nullptr;
}


void PlayerSwingingState::Update(const float deltaTime)
{
	// Increase time for lerp if sliding down rope.
	if (!canJump_)
	{
		timeOnSegment_ += rappelSpeed_ * deltaTime;
	}

	// Store prev position for launch trajectory.
	previousPosition_ = static_cast<float2>(player_.shape_->start_);
	player_.shape_->SetCenter(rope_->GetPositionOnRope(attachmentPoint_, timeOnSegment_));

	// If rope allowed time to exceed 1, player is on last point (no more rope to slide down).
	if (!canJump_ && timeOnSegment_ > 1.0f)
	{
		canJump_ = true;
	}
}


void PlayerSwingingState::Exit()
{
	rope_->SetHasPassenger(false);
	player_.gravityModifier_ = 1.0f;
}


void PlayerSwingingState::StoreRewindData(PlayerStateRewindData* rewindData)
{
	rewindData->vFloat2_ = previousPosition_;
	rewindData->vRopePtr_ = rope_;
	rewindData->vFloatA_ = timeOnSegment_;
	rewindData->vInt_ = attachmentPoint_;
}