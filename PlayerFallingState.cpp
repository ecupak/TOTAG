// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerFallingState.h"

#include "Player.h"
#include "Hitbox.h"

#include "PlayerStandingState.h"
#include "PlayerClimbingState.h"
#include "PlayerZipliningState.h"

#include "ClimbCommand.h"
#include "MoveCommand.h"
#include "JumpCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TilesetData.h"
#include "TileCollision.h"

#include "PlayerStateRewindData.h"


PlayerFallingState::PlayerFallingState(Player& player)
	: PlayerState{ player }
{	}


void PlayerFallingState::Enter(const bool)
{
	name_ = "Fall";
	stateIndex_ = Player::StateName::Falling;
	player_.SetSprite(stateIndex_);
	
	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Rope;

	SetHitboxData();

	player_.GetAllTileMasks();
	
	player_.friction_ = player_.airFriction_;

	// If we are currently in collision with any of these, ignore them until we are out of collision or changed states.
	// This happens when we are in the middle of passing through, but begin to fall.
	missingCollisionMasks_[0] = TileCollisionID::Ledge;
	missingCollisionMasks_[1] = TileCollisionID::Trampoline;
	missingCollisionMasks_[2] = (TileCollisionID::ZiplineR | TileCollisionID::ZiplineL);

	AddMissingCollisionMask();
}


void PlayerFallingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Edit(5, 0, 11, 4,
		TileCollisionID::Nothing,
		Hitbox::Mode::InteractPlus
	);

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Cliff,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Edit(5, 4, 6, 12,
		TileCollisionID::Ladder,
		Hitbox::Mode::InteractPlus
	);

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Cliff,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerFallingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	// Check for ladder.
	const MapTile* mapTile{ nullptr };
	if (player_.canGrabLadder_ && player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Core, TileCollisionID::Ladder, mapTile))
	{
   		if (mapTile != nullptr)
		{
     			return new PlayerClimbingState{ player_, mapTile->position };
		}
	}

	// Check for zipline.
	mapTile = nullptr;
	if (player_.canGrabZipline_ && player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Head, TileCollisionID::ZiplineL | TileCollisionID::ZiplineR, mapTile))
	{
		
		return new PlayerZipliningState{ player_, *mapTile };
	}

	// Aerial movement.
	if (km.IsPressed(Action::Left) != km.IsPressed(Action::Right)) // XOR
	{
		command = new MoveCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };
	}

	// Land on the ground.
	if (player_.isOnGround_)
	{
		return new PlayerStandingState{ player_ };
	}

	// Continue falling.
	return nullptr;
}


// Include missing collision masks if we are no longer colliding with them.
void PlayerFallingState::Update(const float)
{
	AddMissingCollisionMask();
}


void PlayerFallingState::Exit() { }


void PlayerFallingState::StoreRewindData(PlayerStateRewindData*) {}


void PlayerFallingState::AddMissingCollisionMask()
{
	for (int missingCollisionMask : missingCollisionMasks_)
	{ 
		if (~player_.tileMask_ & missingCollisionMask) // if missing mask is not part of mask
		{
			int hitboxFlags{ 0 };

			// Add zipline detection once not touching one anymore.
			if (missingCollisionMask & (TileCollisionID::ZiplineR | TileCollisionID::ZiplineL))
			{
				hitboxFlags = (1 << Player::HitboxRole::Head);
				if (!(player_.AreHitboxesTouchingInteractTile(hitboxFlags, missingCollisionMask)))
				{
					player_.AddTileMask(hitboxFlags, missingCollisionMask);
				}
			}

			// Add trampoline detection once not touching one anymore.
			else if (missingCollisionMask & TileCollisionID::Trampoline)
			{
				if (!(player_.overlappingTileLayers_ & missingCollisionMask))
				{
					hitboxFlags = (1 << Player::HitboxRole::Body) | (1 << Player::HitboxRole::Tail);
					player_.AddTileMask(hitboxFlags, missingCollisionMask);
				}
			}

			// Add ledge detection once not touching one anymore.
			else if (missingCollisionMask & TileCollisionID::Ledge)
			{
				if (!(player_.overlappingTileLayers_ & missingCollisionMask))
				{
					hitboxFlags = (1 << Player::HitboxRole::Body) | (1 << Player::HitboxRole::Tail);
					player_.AddTileMask(hitboxFlags, missingCollisionMask);
				}
			}
		}
	}
}