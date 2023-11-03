// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "KnightWalkingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Knight.h"
#include "Hitbox.h"

#include "KnightAttackingState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


KnightWalkingState::KnightWalkingState(Knight& knight)
	: knight_{ knight }
{	}


KnightWalkingState::KnightWalkingState(Knight& knight, EnemyStateRewindData* rewindData)
	: knight_{ knight }
	, elapsedFrameTime_{ rewindData->vFloatA_ }
{	}


void KnightWalkingState::Enter(const bool)
{
	name_ = "Walk";
	stateIndex_ = Knight::SpriteSheet::Walking;
	knight_.SetSprite(stateIndex_);

	SetHitboxData();

	knight_.GetAllTileMasks();

	knight_.friction_ = knight_.groundFriction_;
}


void KnightWalkingState::SetHitboxData()
{
	knight_.hitboxes_[Knight::HitboxRole::Head].Edit(27, 8, 9, 13,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Body].Edit(24, 20, 13, 12,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Arm].Edit(36, 21, 8, 4,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::ObstacleDetector].Edit(49, 16, 3, 3,
		TileCollisionID::Nothing,
		Hitbox::Mode::Map);

	knight_.hitboxes_[Knight::HitboxRole::GapDetector].Edit(49, 33, 3, 3,
		TileCollisionID::Nothing,
		Hitbox::Mode::Map);

	knight_.hitboxes_[Knight::HitboxRole::PlayerDetector].Edit(24, 16, 40, 16,
		TileCollisionID::Nothing,
		Hitbox::Mode::None);
}


EnemyState* KnightWalkingState::HandleInput(const Action& action, Command*& command)
{
	// Check if player is within detector.
	const Hitbox& hitbox{ knight_.hitboxes_[Knight::HitboxRole::PlayerDetector] };
	const float2& position{ static_cast<float2>(knight_.shape_->start_) };

	if (knight_.playerPosition_->x > position.x + hitbox.offset_.x
		&& knight_.playerPosition_->x < position.x + hitbox.offset_.x + hitbox.size_.x
		&& knight_.playerPosition_->y > position.y + hitbox.offset_.y
		&& knight_.playerPosition_->y < position.y + hitbox.offset_.y + hitbox.size_.y)
	{
		return new KnightAttackingState{ knight_ };
	}


	// Walk.
	if ((action == Action::Left) != (action == Action::Right)) // XOR
	{
		// If taking a step in the direction already facing, check obstacle detector first. If obstacle detected, go opposite direction.
		if ((action == Action::Left && knight_.isFlipped_) || (action == Action::Right && !knight_.isFlipped_))
		{
			const bool isFacingObstacle{ knight_.IsHitboxTouchingMapTile(Knight::HitboxRole::ObstacleDetector, TileCollisionID::Obstacle | TileCollisionID::Spike) };
			const bool isFacingGap{ !knight_.IsHitboxTouchingMapTile(Knight::HitboxRole::GapDetector, TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline) };

			// Turn early. Reset state duration and action.
			if (isFacingObstacle || isFacingGap)
			{
				knight_.stateDuration_ = 2.0f;
				knight_.currentAction_ = (action == Action::Left ? Action::Right : Action::Left);

			}
		}

		command = new MoveCommand{ &knight_, GetOpposingDirectionsAsValue(knight_.currentAction_) };

		return nullptr;
	}


	// Keep walking.
	return nullptr;
}


void KnightWalkingState::Update(const float deltaTime)
{
	knight_.stateDuration_ -= deltaTime;

	if (knight_.velocity_->x != 0)
	{
		knight_.SetSpriteFacing(knight_.velocity_->x < 0);
	}

	if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
	{
		elapsedFrameTime_ = 0.0f;
		knight_.sprite_->AdvanceToNextFrame();
	}
}


void KnightWalkingState::Exit() {}


void KnightWalkingState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
}