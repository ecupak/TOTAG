// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ArcherStandingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Archer.h"
#include "Hitbox.h"

#include "ArcherWalkingState.h"
#include "ArcherAlertState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"

#include "EnemyStateRewindData.h"


ArcherStandingState::ArcherStandingState(Archer& archer)
	: archer_{ archer }
{	}


void ArcherStandingState::Enter(const bool)
{
	name_ = "Stand";
	stateIndex_ = Archer::SpriteSheet::Standing;
	archer_.SetSprite(stateIndex_);

	SetHitboxData();

	archer_.GetAllTileMasks();

	archer_.friction_ = archer_.groundFriction_;
}


void ArcherStandingState::SetHitboxData()
{
	archer_.hitboxes_[Archer::HitboxRole::Head].Edit(15, 9, 10, 9,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	archer_.hitboxes_[Archer::HitboxRole::Body].Edit(13, 16, 14, 8,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	archer_.hitboxes_[Archer::HitboxRole::PersonalSpace].Edit(11, 16, 18, 8,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle
	);

	archer_.hitboxes_[Archer::HitboxRole::ObstacleDetector].Edit(28, 16, 3, 3,
		TileCollisionID::Obstacle | TileCollisionID::Spike,
		Hitbox::Mode::Map
	);

	archer_.hitboxes_[Archer::HitboxRole::GapDetector].Edit(28, 26, 3, 3,
		TileCollisionID::Nothing,
		Hitbox::Mode::Map
	);
	
	archer_.hitboxes_[Archer::HitboxRole::PlayerDetector].Edit(19, -30, 96, 30 + static_cast<int>(archer_.shape_->size_.y),
		TileCollisionID::Nothing,
		Hitbox::Mode::None
	);
}


EnemyState* ArcherStandingState::HandleInput(const Action& action, Command*& command)
{
	// Check if player is within detector.
	if (archer_.attackDelay_ <= 0.0f)
	{
		const Hitbox& hitbox{ archer_.hitboxes_[Archer::HitboxRole::PlayerDetector] };
		const float2& position{ static_cast<float2>(archer_.shape_->start_) };

		if (archer_.playerPosition_->x > position.x + hitbox.offset_.x
			&& archer_.playerPosition_->x < position.x + hitbox.offset_.x + hitbox.size_.x
			&& archer_.playerPosition_->y > position.y + hitbox.offset_.y
			&& archer_.playerPosition_->y < position.y + hitbox.offset_.y + hitbox.size_.y)
		{
			archer_.stateDuration_ = 0.5f;
			archer_.attackDelay_ = 1.0f;

			return new ArcherAlertState{ archer_ };
		}
	}
	

	// Walk.
	if ((action == Action::Left) != (action == Action::Right)) // XOR
	{
		command = new MoveCommand{ &archer_, GetOpposingDirectionsAsValue(action) };

		return new ArcherWalkingState{ archer_ };
	}


	// Stand.	
	return nullptr;
}


void ArcherStandingState::Update(const float deltaTime) 
{
	archer_.stateDuration_ -= deltaTime;
}


void ArcherStandingState::Exit() {}


void ArcherStandingState::StoreRewindData(EnemyStateRewindData*) {}