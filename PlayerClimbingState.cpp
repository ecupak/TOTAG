// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
#include "precomp.h"
#include "PlayerClimbingState.h"

#include "Player.h"
#include "Hitbox.h"
#include "Shape.h"
#include "TileCollision.h"

#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerStandingState.h"

#include "ClimbCommand.h"
#include "MoveCommand.h"
#include "JumpCommand.h"
#include "LaunchCommand.h"

#include "KeyboardManager.h"
#include "Vector2.h"

#include "PlayerStateRewindData.h"


PlayerClimbingState::PlayerClimbingState(Player& player, const int2& ropePosition)
	: PlayerState{ player }
	, ropeCenterX_{ static_cast<float>(ropePosition.x + (TILE_WIDTH >> 1)) }
{	}


PlayerClimbingState::PlayerClimbingState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, ropeCenterX_{ rewindData->vFloatA_ }
	, grabTimer_{ rewindData->vFloatB_ }
	, doesPlayerHaveControl_{ grabTimer_ < 1.0f }
{	}


void PlayerClimbingState::Enter(const bool)
{
	name_ = "Climb";
	stateIndex_ = Player::StateName::Climbing;
	player_.SetSprite(stateIndex_);

	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack;

	SetHitboxData();

	player_.GetAllTileMasks();

	// Adjust gravity. Does not care about friction (no x movement).
	player_.gravityModifier_ = 0.0f;

	// Cancel player velocity.
	*player_.velocity_ = float2{ 0.0f ,0.0f };
}


void PlayerClimbingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Edit(5, 0, 11, 4,
		TileCollisionID::Ladder,
		Hitbox::Mode::Interact
	);

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	// Use the core to detect obstacles. Player is not always perfectly aligned with the tile when climbing up,
	// and can get stuck when trying to climb over a cliff tile while sandwiched between 2 obstacle tiles.
	player_.hitboxes_[Player::HitboxRole::Core].Edit(5, 4, 6, 12,
		TileCollisionID::Obstacle | TileCollisionID::Ladder,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Interact
	);

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerClimbingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	if (!doesPlayerHaveControl_)
	{
		return nullptr;
	}

	// If we've climbed low enough to touch the ground, get off.
	if (player_.isOnGround_)
	{
		return new PlayerStandingState{ player_ };
	}

	// Check if we are still on a rope. Fall if not.
	if (!(player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Core, TileCollisionID::Ladder)))
	{
		return new PlayerFallingState{ player_ };
	}

	// Jump. Will have a delay before player can grab a ladder again. Allows player to get away from current ladder and prevents jump abuse.
	if (km.IsJustPressed(Action::Jump) && !(player_.overlappingTileLayers_ & TileCollisionID::Cliff))
	{
		player_.canGrabLadder_ = false;
		player_.ladderDelay_ = 0.6f;

		command = new LaunchCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)), ladderJumpImpulse_ };

		return new PlayerJumpingState{ player_, true };
	}

	// Move along the rope. If no input, stay at position.
	command = new ClimbCommand{ player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Up), km.IsPressed(Action::Down)) };

	// Possibly allow left/right to make climbing sprite look in that direction.

	return nullptr;
}


void PlayerClimbingState::Update(const float deltaTime)
{
	if (grabTimer_ < 1.0f)
	{
		grabTimer_ += deltaTime * grabSpeed_;

		player_.shape_->SetCenter(
			float2{ 
				ec::Lerpf(player_.shape_->center_.x, ropeCenterX_, grabTimer_),
				player_.shape_->center_.y 
			}
		);
	}
	else
	{
		doesPlayerHaveControl_ = true;
	}
}


// Restore gravity.
void PlayerClimbingState::Exit() 
{
	player_.gravityModifier_ = 1.0f;
}


void PlayerClimbingState::StoreRewindData(PlayerStateRewindData* rewindData)
{
	rewindData->vFloatA_ = ropeCenterX_;
	rewindData->vFloatB_ = grabTimer_;
}