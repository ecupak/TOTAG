// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ArcherAlertState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Archer.h"
#include "Hitbox.h"

#include "ArcherStandingState.h"
#include "ArcherShootingState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


ArcherAlertState::ArcherAlertState(Archer& archer)
	: archer_{ archer }
{	}


ArcherAlertState::ArcherAlertState(Archer& archer, EnemyStateRewindData* rewindData)
	: archer_{ archer }
	, elapsedFrameTime_{ rewindData->vFloatA_}
	, elapsedAlertTime_{ rewindData->vFloatB_ }
{	}


void ArcherAlertState::Enter(const bool isRewindEntry)
{
	name_ = "Alert";
	stateIndex_ = Archer::SpriteSheet::Shooting;
	archer_.SetSprite(stateIndex_);

	SetHitboxData();
	
	archer_.GetAllTileMasks();

	archer_.friction_ = archer_.groundFriction_;
	
	if (!isRewindEntry)
	{
		archer_.sprite_->SetFrame(0);
		archer_.lockonSpriteFrame_ = 0;
	}
}


void ArcherAlertState::SetHitboxData()
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

	archer_.hitboxes_[Archer::HitboxRole::ObstacleDetector].Ignore();

	archer_.hitboxes_[Archer::HitboxRole::GapDetector].Ignore();

	archer_.hitboxes_[Archer::HitboxRole::PlayerDetector].Ignore();
}


EnemyState* ArcherAlertState::HandleInput(const Action&, Command*&)
{
	// Stay alert while player is in view.
	const Hitbox& hitbox{ archer_.hitboxes_[Archer::HitboxRole::PlayerDetector] };
	const float2& position{ static_cast<float2>(archer_.shape_->start_) };
	const bool isPlayerInView{ archer_.playerPosition_->x > position.x + hitbox.offset_.x
		&& archer_.playerPosition_->x < position.x + hitbox.offset_.x + hitbox.size_.x
		&& archer_.playerPosition_->y > position.y + hitbox.offset_.y
		&& archer_.playerPosition_->y < position.y + hitbox.offset_.y + hitbox.size_.y
	};
	
	if (!isPlayerInView)
	{
		archer_.lockonSpriteFrame_ = -1;

		return new ArcherStandingState{ archer_ };
	}

	// If alert for 1 second, shoot player.
	if (elapsedAlertTime_ > alertTime_)
	{
		archer_.lockonSpriteFrame_ = 11;
		archer_.sprite_->AdvanceToNextFrame();

		return new ArcherShootingState{ archer_ };
	}

	// Else, stay alert.
	return nullptr;
}


void ArcherAlertState::Update(const float deltaTime)
{
	// Advance to aiming sprite frame.
	if (archer_.sprite_->GetFrame() < 3)
	{
		if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
		{
			elapsedFrameTime_ = 0.0f;
			archer_.sprite_->AdvanceToNextFrame();
		}
	}
	
	// Countdown frame is based on duration of alert state. Should be fps independent.
	elapsedAlertTime_ += deltaTime;
	int lockonFrame = static_cast<int>(ec::Lerpf(0.0f, 10.0f, elapsedAlertTime_ / alertTime_));
	archer_.lockonSpriteFrame_ = lockonFrame;
}


void ArcherAlertState::Exit() {}


void ArcherAlertState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
	rewindData->vFloatB_ = elapsedAlertTime_;
}