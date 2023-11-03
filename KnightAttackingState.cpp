// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "KnightAttackingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Knight.h"
#include "Hitbox.h"

#include "KnightWalkingState.h"

#include "MoveCommand.h"

#include "KeyboardManager.h"
#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


KnightAttackingState::KnightAttackingState(Knight& knight)
	: knight_{ knight }
{	}


KnightAttackingState::KnightAttackingState(Knight& knight, EnemyStateRewindData* rewindData)
	: knight_{ knight }
	, elapsedFrameTime_{ rewindData->vFloatA_ }
	, isAttackDone_{ rewindData->vBool_ }
{	}


void KnightAttackingState::Enter(const bool)
{
	name_ = "Attack";
	stateIndex_ = Knight::SpriteSheet::Attacking;
	knight_.SetSprite(stateIndex_);

	SetHitboxData();

	knight_.GetAllTileMasks();

	knight_.friction_ = knight_.groundFriction_;
}


void KnightAttackingState::SetHitboxData()
{
	knight_.hitboxes_[Knight::HitboxRole::Head].Edit(27, 8, 11, 14,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Body].Edit(24, 19, 16, 13,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Arm].Edit(39, 24, 20, 7,
		TileCollisionID::Nothing,
		Hitbox::Mode::None);	// Only object mode on frame with attack animation!

	knight_.hitboxes_[Knight::HitboxRole::ObstacleDetector].Ignore();

	knight_.hitboxes_[Knight::HitboxRole::GapDetector].Ignore();

	knight_.hitboxes_[Knight::HitboxRole::PlayerDetector].Ignore();
}


EnemyState* KnightAttackingState::HandleInput(const Action&, Command*&)
{
	if (isAttackDone_)
	{
		return new KnightWalkingState{ knight_ };
	}
	else 
	{
		return nullptr;
	}
}


void KnightAttackingState::Update(const float deltaTime)
{
	if ((elapsedFrameTime_ += deltaTime) >= frameTime_)
	{
		elapsedFrameTime_ = 0.0f;
		knight_.sprite_->AdvanceToNextFrame();

		switch (knight_.sprite_->GetFrame())
		{
		// Only do damage on attack frame.
		case 4:
			knight_.hitboxes_[Knight::HitboxRole::Arm].mode_ = Hitbox::Mode::Object;
			break;
		// Disable attack hitbox.
		case 5:
			knight_.hitboxes_[Knight::HitboxRole::Arm].Ignore();
			break;
		// Finish attack.
		case 7:
			isAttackDone_ = true;
			break;
		default:
			break;
		}
	}
}


void KnightAttackingState::Exit() {}


void KnightAttackingState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedFrameTime_;
	rewindData->vBool_ = isAttackDone_;
}