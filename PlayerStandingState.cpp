// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerStandingState.h"

#include "Player.h"
#include "Hitbox.h"
#include "Shape.h"

#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerClimbingState.h"
#include "PlayerFallingState.h"
#include "PlayerBouncingState.h"

#include "ClimbCommand.h"
#include "MoveCommand.h"
#include "JumpCommand.h"
#include "ThrowRockCommand.h"

#include "KeyboardManager.h"
#include "Level.h"
#include "MapData.h"
#include "TileCollision.h"

#include "PlayerStateRewindData.h"


PlayerStandingState::PlayerStandingState(Player& player)
	: PlayerState{ player }
{	}


void PlayerStandingState::Enter(const bool)
{
	name_ = "Stand";
	stateIndex_ = Player::StateName::Standing;
	player_.SetSprite(stateIndex_);

	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Trigger;

	SetHitboxData();

	player_.GetAllTileMasks();

	player_.friction_ = player_.groundFriction_;
}


void PlayerStandingState::SetHitboxData()
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


PlayerState* PlayerStandingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	/* Check for automatic state changes first */
	
	// If player is moving up or down while they should be standing, start falling.
	if (player_.velocity_->y != 0.0f)
	{
		return new PlayerFallingState{ player_ };
	}


	// Check feet for trampoline.
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
	
	
	/* Check for delibrate state changes next */

	// Jump.
	if (km.IsPressed(Action::Jump))
	{
		// Force facing even if jumps are strung together and skip the walking state.
		if (GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)))
		{
			player_.SetSpriteFacing(km.IsPressed(Action::Left));
		}

		command = new JumpCommand{ &player_, true, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };

		return new PlayerJumpingState{ player_, true };
	}


	// TODO: Make a superstate that checks for walking and throwing. Will call it here between the above calls and a nullptr return.
	// Throw.
	if (km.IsPressed(Action::Throw) && player_.canThrow_)
	{
		command = new ThrowRockCommand{ player_ };

		player_.canThrow_ = false;
		player_.throwDelay_ = 0.5f;

		return nullptr;
	}

	
	// Walk.
	if (km.IsPressed(Action::Left) != km.IsPressed(Action::Right)) // XOR
	{	
		command = new MoveCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };
			
		return new PlayerWalkingState{ player_ };
	}


	// Stand.	
	return nullptr;
}


void PlayerStandingState::Update(const float) {}


void PlayerStandingState::Exit() {}


void PlayerStandingState::StoreRewindData(PlayerStateRewindData*) {}