// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerWalkingState.h"

#include "Player.h"
#include "Vector2.h"
#include "Hitbox.h"

#include "PlayerStandingState.h"
#include "PlayerJumpingState.h"
#include "PlayerClimbingState.h"
#include "PlayerFallingState.h"
#include "PlayerBouncingState.h"

#include "ClimbCommand.h"
#include "MoveCommand.h"
#include "JumpCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"

#include "PlayerStateRewindData.h"


PlayerWalkingState::PlayerWalkingState(Player& player)
	: PlayerState{ player }
{	}


PlayerWalkingState::PlayerWalkingState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, elapsedFrameTime_{ rewindData->vFloatA_ }
{	}
	

void PlayerWalkingState::Enter(const bool)
{
	name_ = "Walk";
	stateIndex_ = Player::StateName::Walking;
	player_.SetSprite(stateIndex_);

	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Trigger;

	SetHitboxData();

	player_.GetAllTileMasks();

	player_.friction_ = player_.groundFriction_;
}


void PlayerWalkingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Ignore();

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline | TileCollisionID::Spike,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Ignore();

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline | TileCollisionID::Spike,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Edit(5, 16, 11, 3,
		TileCollisionID::Ladder | TileCollisionID::Trampoline,
		Hitbox::Mode::Interact
	);
}


PlayerState* PlayerWalkingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	// Immediately go into a falling state if our velocity is not zero (we are not on ground).
	if (player_.velocity_->y != 0.0f)
	{
		return new PlayerFallingState{ player_ };
	}

	// Check for trampoline.
	const MapTile* mapTile{ nullptr };
	if (player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Feet, TileCollisionID::Trampoline, mapTile))
	{
		if (mapTile != nullptr)
		{
			return new PlayerBouncingState{ player_ };
		}
	}

	// Check for rope.
	mapTile = nullptr;
	if (km.IsJustPressed(Action::Down))
	{
		if (player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Feet, TileCollisionID::Ladder, mapTile))
		{
			command = new ClimbCommand{ player_, 1 };

			return new PlayerClimbingState{ player_, mapTile->position };
		}
	}

	// Jump.
	if (km.IsPressed(Action::Jump))
	{
		command = new JumpCommand{ &player_, true, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };

		return new PlayerJumpingState{ player_, true };
	}

	// Walk.
	if (km.IsPressed(Action::Left) != km.IsPressed(Action::Right)) // XOR
	{
		command = new MoveCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };

		return nullptr;
	}

	// Stand.
	return new PlayerStandingState{ player_ };
}


void PlayerWalkingState::Update(const float deltaTime)
{
	player_.SetSpriteFacing(player_.velocity_->x < 0);

	if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
	{
		elapsedFrameTime_ = 0.0f;
		player_.sprite_->AdvanceToNextFrame();
	}
}


void PlayerWalkingState::Exit() {}


void PlayerWalkingState::StoreRewindData(PlayerStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
}