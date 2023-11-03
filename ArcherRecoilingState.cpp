// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ArcherRecoilingState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Archer.h"
#include "Hitbox.h"

#include "ArcherStandingState.h"

#include "MoveCommand.h"

#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


ArcherRecoilingState::ArcherRecoilingState(Archer& archer, const float2 collisionNormals)
	: archer_{ archer }
	, collisionNormals_{ collisionNormals }
{	}


ArcherRecoilingState::ArcherRecoilingState(Archer& archer, EnemyStateRewindData* rewindData)
	: archer_{ archer }
	, elapsedRecoilTime_{ rewindData->vFloatA_ }
	, isRecoilActive_{ elapsedRecoilTime_ < maxRecoilTime_ }
{	}


void ArcherRecoilingState::Enter(const bool isRewindEntry)
{
	name_ = "Recoil";
	stateIndex_ = Archer::SpriteSheet::Recoiling;
	archer_.SetSprite(stateIndex_);
	
	// Extend delay. Don't set, because if hit out of attack archer resumes it fast.
	archer_.attackDelay_ += 0.2f;

	SetHitboxData();

	archer_.GetAllTileMasks();

	archer_.friction_ = archer_.groundFriction_;
	
	if (!isRewindEntry)
	{
		archer_.lockonSpriteFrame_ = -1;

		// Set velocity to recoil velocity.
		*archer_.velocity_ = GetRecoilVelocity();
	}
}


float2 ArcherRecoilingState::GetRecoilVelocity()
{
	float2 recoilVelocity{
		normalize(
			float2{
				// Use collision normal if not 0.
				(collisionNormals_.x != 0.0f
					? collisionNormals_.x
					// Else, use opposite of facing.
					: archer_.sprite_->IsFlipped()
						? 1.0f
						: -1.0f
				),
				-1.0f
			}
		)
	};

	// Enemies do not move left/right with recoil This prevents being knocked into holes or spikes.
	return { 0.0f, recoilVelocity.y * recoilIntensity_ };
}


void ArcherRecoilingState::SetHitboxData()
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


EnemyState* ArcherRecoilingState::HandleInput(const Action&, Command*&)
{
	if (!isRecoilActive_ && archer_.isOnGround_)
	{
		return new ArcherStandingState{ archer_ };
	}
	else
	{
		return nullptr;
	}
}


void ArcherRecoilingState::Update(const float deltaTime)
{
	if (elapsedRecoilTime_ < maxRecoilTime_)
	{
		elapsedRecoilTime_ += deltaTime;
	}
	else
	{
		isRecoilActive_ = false;
	}
}


void ArcherRecoilingState::Exit() {}


void ArcherRecoilingState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedRecoilTime_;
}