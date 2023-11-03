// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ArcherShootingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Archer.h"
#include "Hitbox.h"

#include "ArrowPool.h"

#include "ArcherStandingState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


ArcherShootingState::ArcherShootingState(Archer& archer)
	: archer_{ archer }
{	}


ArcherShootingState::ArcherShootingState(Archer& archer, EnemyStateRewindData* rewindData)
	: archer_{ archer }
	, elapsedFrameTime_{ rewindData->vFloatA_ }
	, isAttackDone_{ rewindData->vBool_ }
{	}


void ArcherShootingState::Enter(const bool isRewindEntry)
{
	name_ = "Shoot";
	stateIndex_ = Archer::SpriteSheet::Shooting;
	archer_.SetSprite(stateIndex_);

	SetHitboxData();

	archer_.GetAllTileMasks();

	archer_.friction_ = archer_.groundFriction_;
	
	if (!isRewindEntry)
	{
		ShootArrow();
	}
}


void ArcherShootingState::SetHitboxData()
{
	archer_.hitboxes_[Archer::HitboxRole::Head].Edit(14, 9, 10, 8,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	archer_.hitboxes_[Archer::HitboxRole::Body].Edit(11, 16, 16, 8,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object
	);

	archer_.hitboxes_[Archer::HitboxRole::PersonalSpace].Edit(11, 16, 18, 8,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle
	);

	archer_.hitboxes_[Archer::HitboxRole::ObstacleDetector].Ignore();

	archer_.hitboxes_[Archer::HitboxRole::GapDetector].Ignore();

	archer_.hitboxes_[Archer::HitboxRole::PlayerDetector].Ignore();
}


EnemyState* ArcherShootingState::HandleInput(const Action&, Command*&)
{
	if (isAttackDone_)
	{
		archer_.lockonSpriteFrame_ = -1;
		archer_.attackDelay_ = 2.0f;

		return new ArcherStandingState{ archer_ };
	}
	else
	{
		return nullptr;
	}
}


void ArcherShootingState::Update(const float deltaTime)
{
	if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
	{
		elapsedFrameTime_ = 0.0f;
		archer_.sprite_->AdvanceToNextFrame();

		switch (archer_.sprite_->GetFrame())
		{
		// Finish attack.
		case 6:
			isAttackDone_ = true;
			break;
		default:
			break;
		}
	}
}

void ArcherShootingState::ShootArrow()
{
	const float2 direction{ normalize(static_cast<float2>(*archer_.playerPosition_ - archer_.shape_->center_)) };
	const float speed{ 300.0f };

	// Create arrow. Send info so it spawns in front of archer along trajectory to target.
	archer_.arrowPool_->Create(static_cast<float2>(archer_.shape_->center_), archer_.hitboxes_[Archer::HitboxRole::PersonalSpace].size_.x, direction, speed);
}


void ArcherShootingState::Exit() {}


void ArcherShootingState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
	rewindData->vBool_ = isAttackDone_;
}