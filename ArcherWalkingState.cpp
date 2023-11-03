// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ArcherWalkingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Archer.h"
#include "Hitbox.h"

#include "ArcherStandingState.h"
#include "ArcherAlertState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


ArcherWalkingState::ArcherWalkingState(Archer& archer)
	: archer_{ archer }
{	}


ArcherWalkingState::ArcherWalkingState(Archer& archer, EnemyStateRewindData* rewindData)
	: archer_{ archer }
	, elapsedFrameTime_{ rewindData->vFloatA_ }
{	}


void ArcherWalkingState::Enter(const bool)
{
	name_ = "Walk";
	stateIndex_ = Archer::SpriteSheet::Walking;
	archer_.SetSprite(stateIndex_);

	SetHitboxData();

	archer_.GetAllTileMasks();

	archer_.friction_ = archer_.groundFriction_;
}


void ArcherWalkingState::SetHitboxData()
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

EnemyState* ArcherWalkingState::HandleInput(const Action& action, Command*& command)
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
		// If taking a step in the direction already facing, check obstacle detector first. If obstacle detected, go opposite direction.
		if ((action == Action::Left && archer_.isFlipped_) || (action == Action::Right && !archer_.isFlipped_))
		{
			const bool isFacingObstacle{ archer_.IsHitboxTouchingMapTile(Archer::HitboxRole::ObstacleDetector, TileCollisionID::Obstacle | TileCollisionID::Spike) };
			const bool isFacingGap{ !archer_.IsHitboxTouchingMapTile(Archer::HitboxRole::GapDetector, TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline) };
			
			if (isFacingObstacle || isFacingGap)
			{
				archer_.currentAction_ = (action == Action::Left ? Action::Right : Action::Left);
			}
		}

		command = new MoveCommand{ &archer_, GetOpposingDirectionsAsValue(archer_.currentAction_) };

		return nullptr;
	}


	// Stand.
	return new ArcherStandingState{ archer_ };
}


void ArcherWalkingState::Update(const float deltaTime)
{
	archer_.stateDuration_ -= deltaTime;

	archer_.SetSpriteFacing(archer_.velocity_->x < 0);

	if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
	{
		elapsedFrameTime_ = 0.0f;
		archer_.sprite_->AdvanceToNextFrame();
	}
}


void ArcherWalkingState::Exit() {}


void ArcherWalkingState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
}