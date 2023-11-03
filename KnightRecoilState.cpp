// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "KnightRecoilState.h"

#include "Shape.h"
#include "Enemy.h"
#include "Knight.h"
#include "Hitbox.h"

#include "KnightWalkingState.h"

#include "MoveCommand.h"

#include "MapData.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "EnemyStateRewindData.h"


KnightRecoilState::KnightRecoilState(Knight& knight, const float2 collisionNormals)
	: knight_{ knight }
	, collisionNormals_{ collisionNormals }
{	}


KnightRecoilState::KnightRecoilState(Knight& knight, EnemyStateRewindData* rewindData)
	: knight_{ knight }
	, elapsedRecoilTime_{ rewindData->vFloatA_ }
	, isRecoilActive_{ elapsedRecoilTime_ < maxRecoilTime_ }
{	}


void KnightRecoilState::Enter(const bool isRewindEntry)
{
	name_ = "Recoil";
	stateIndex_ = Knight::SpriteSheet::Recoiling;
	knight_.SetSprite(stateIndex_);

	SetHitboxData();

	knight_.GetAllTileMasks();

	// Change friction.
	knight_.friction_ = knight_.groundFriction_;

	if (!isRewindEntry)
	{
		// Set velocity to recoil velocity.
		*knight_.velocity_ = GetRecoilVelocity();

		// Activate invulnerability.
		knight_.isInvulnerable_ = true;
	}
}


float2 KnightRecoilState::GetRecoilVelocity()
{
	float2 recoilVelocity{
		normalize(
			float2{
				// Use collision normal if not 0.
				(collisionNormals_.x != 0.0f
					? collisionNormals_.x
					// Else, use opposite of facing.
					: knight_.sprite_->IsFlipped()
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


void KnightRecoilState::SetHitboxData()
{
	knight_.hitboxes_[Knight::HitboxRole::Head].Edit(27, 8, 11, 14,
		TileCollisionID::Nothing,
		Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Body].Edit(24, 19, 16, 13,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object);

	knight_.hitboxes_[Knight::HitboxRole::Arm].Edit(39, 24, 20, 7,
		TileCollisionID::Nothing,
		Hitbox::Mode::None);

	knight_.hitboxes_[Knight::HitboxRole::ObstacleDetector].Edit(33, 16, 3, 3,
		TileCollisionID::Nothing,
		Hitbox::Mode::Map);

	knight_.hitboxes_[Knight::HitboxRole::GapDetector].Edit(33, 33, 3, 3,
		TileCollisionID::Nothing,
		Hitbox::Mode::Map);

	knight_.hitboxes_[Knight::HitboxRole::PlayerDetector].Edit(8, 0, 40, 32,
		TileCollisionID::Nothing,
		Hitbox::Mode::None);
}


EnemyState* KnightRecoilState::HandleInput(const Action&, Command*&)
{
	if (!isRecoilActive_ && knight_.isOnGround_)
	{
		return new KnightWalkingState{ knight_ };
	}
	else
	{
		return nullptr;
	}
}


void KnightRecoilState::Update(const float deltaTime)
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


void KnightRecoilState::Exit() {}


void KnightRecoilState::StoreRewindData(EnemyStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedRecoilTime_;
}