// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerJumpingState.h"

#include "Player.h"
#include "Hitbox.h"
#include "Vector2.h"

#include "PlayerFallingState.h"
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


PlayerJumpingState::PlayerJumpingState(Player& player, bool isPlayerChoice)
	: PlayerState{ player }
	, isPlayerChoice_{ isPlayerChoice }
{	}


PlayerJumpingState::PlayerJumpingState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, jumpTimer_{ rewindData->vFloatA_ }
	, isPlayerChoice_{ rewindData->vBool_ }
{	
	player_.gravityModifier_ = rewindData->vFloatB_;
}


void PlayerJumpingState::Enter(const bool isRewindEntry)
{	
	name_ = "Jump";
	stateIndex_ = Player::StateName::Jumping;
	player_.SetSprite(stateIndex_);

	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Rope;

	SetHitboxData();

	player_.GetAllTileMasks();

	player_.friction_ = player_.airFriction_;
	
	if (!isRewindEntry)
	{
		// Only turn gravity on after player stops holding jump or max jump height reached.
		player_.gravityModifier_ = 0.0f;
	}
}


void PlayerJumpingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Edit(5, 0, 11, 4,
		TileCollisionID::ZiplineL | TileCollisionID::ZiplineR,
		Hitbox::Mode::InteractPlus
	);

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Spike,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Edit(5, 4, 6, 12,
		TileCollisionID::Ladder,
		Hitbox::Mode::InteractPlus
	);

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Spike,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerJumpingState::HandleInput(const KeyboardManager& km, Command*& command)
{
	// If velocity_.y is 0 or greater, we are falling.
	if (player_.velocity_->y >= 0.0f)
	{
		command = new MoveCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };
		
		return new PlayerFallingState{ player_ };
	}

	// Check for ladder/rope.
	const MapTile* mapTile{ nullptr };
	if (player_.canGrabLadder_ && player_.IsHitboxTouchingInteractTile(
		Player::HitboxRole::Core, TileCollisionID::Ladder, mapTile))
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

	// Jump was released. Can no longer gain jump height until landing.
	if (isPlayerChoice_ && !km.IsPressed(Action::Jump))
	{
		player_.gravityModifier_ = 1.0f;
	}

	// Aerial movement while jumping.
	if (player_.velocity_->y < 0.0f)
	{	
		command = new MoveCommand{ &player_, GetOpposingDirectionsAsValue(km.IsPressed(Action::Left), km.IsPressed(Action::Right)) };

		return nullptr;
	}

	return nullptr;
}


void PlayerJumpingState::Update(const float deltaTime)
{	
	// Track how long jump has lasted.
	jumpTimer_ += deltaTime;

	if (jumpTimer_ >= maxJumpTime_)
	{
		player_.gravityModifier_ = 1.0f;
	}	
}


 void PlayerJumpingState::Exit() 
 { 
	 player_.gravityModifier_ = 1.0f;
 }


 void PlayerJumpingState::StoreRewindData(PlayerStateRewindData* rewindData)
 {
	 rewindData->vFloatA_ = jumpTimer_;
	 rewindData->vFloatB_ = player_.gravityModifier_;
	 rewindData->vBool_ = isPlayerChoice_;
 }