// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerBouncingState.h"

#include "Player.h"

#include "Hitbox.h"

#include "PlayerJumpingState.h"

#include "LaunchCommand.h"

#include "KeyboardManager.h"
#include "TileCollision.h"

#include "PlayerStateRewindData.h"


PlayerBouncingState::PlayerBouncingState(Player& player)
	: PlayerState{ player }
{	}


void PlayerBouncingState::Enter(const bool)
{
	name_ = "Bounce";
	stateIndex_ = Player::StateName::Bouncing;
	player_.SetSprite(stateIndex_);

	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack;

	SetHitboxData();

	player_.GetAllTileMasks();
}


void PlayerBouncingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Ignore();

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Ignore();

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle | Hitbox::Mode::InteractPlus | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerBouncingState::HandleInput(const KeyboardManager&, Command*& command)
{
	command = new LaunchCommand{ &player_, {0.0, -170.0f} };

	return new PlayerJumpingState{ player_, false };
}


void PlayerBouncingState::Update(const float) { }


void PlayerBouncingState::Exit() { }


void PlayerBouncingState::StoreRewindData(PlayerStateRewindData*) {}