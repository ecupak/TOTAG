#include "precomp.h"
#include "PlayerRecoilingState.h"

#include "Player.h"
#include "Hitbox.h"

#include "PlayerStandingState.h"
#include "PlayerJumpingState.h"

#include "KeyboardManager.h"
#include "TileCollision.h"
#include "Utility.h"
#include "Vector2.h"

#include "PlayerStateRewindData.h"


PlayerRecoilingState::PlayerRecoilingState(Player& player, const float2 collisionNormals)
	: PlayerState{ player }
	, collisionNormals_{ collisionNormals }
{	}


PlayerRecoilingState::PlayerRecoilingState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, elapsedRecoilTime_{ rewindData->vFloatA_ }
	, isRecoilActive_{ elapsedRecoilTime_ < maxRecoilTime_ }
{	}


void PlayerRecoilingState::Enter(const bool isRewindEntry)
{
	name_ = "Recoil";
	stateIndex_ = Player::StateName::Recoiling;
	player_.SetSprite(stateIndex_);

	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible;

	SetHitboxData();

	player_.GetAllTileMasks();
	
	player_.friction_ = player_.airFriction_;
	
	// If we are currently in collision with this, ignore it until we are out of collision or changed states.
	missingCollisionMask_ = TileCollisionID::Ledge;

	AddMissingCollisionMask();
	
	if (!isRewindEntry)
	{
		// Set player velocity to recoil velocity.
		*player_.velocity_ = GetRecoilVelocity();

		// Activate invulnerability.
		player_.isInvulnerable_ = true;
	}
}


float2 PlayerRecoilingState::GetRecoilVelocity()
{
	bool isFacingLeft{ player_.sprite_->IsFlipped()};

	float2 recoilVelocity{
		normalize(
			float2{
				// Use collision normal if not 0.
				collisionNormals_.x != 0.0f
					? collisionNormals_.x
					// Else, use opposite of facing if no other indicators are available.
					: isFacingLeft
						? 1.0f
						: -1.0f
				,
				-1.0f
			}
		)
	};

	return { recoilVelocity.x * recoilIntensity_, recoilVelocity.y * recoilIntensity_ };
}


void PlayerRecoilingState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Ignore();

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Cliff,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Ignore();

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Cliff,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


// Input is ignored in this state.
PlayerState* PlayerRecoilingState::HandleInput(const KeyboardManager&, Command*&)
{
	if (isRecoilActive_)
	{
		return nullptr;
	}
	else
	{
		if (player_.isOnGround_)
		{
			return new PlayerStandingState{ player_ };
		}
		else
		{
			return new PlayerJumpingState{ player_, false };
		}
	}
};


void PlayerRecoilingState::Update(const float deltaTime)
{
	if (elapsedRecoilTime_ < maxRecoilTime_)
	{
		elapsedRecoilTime_ += deltaTime;
	}
	else
	{
		isRecoilActive_ = false;
	}

	AddMissingCollisionMask();
};


void PlayerRecoilingState::Exit() {};


void PlayerRecoilingState::StoreRewindData(PlayerStateRewindData* rewindData)
{
	rewindData->vFloatA_ = elapsedRecoilTime_;
}


void PlayerRecoilingState::AddMissingCollisionMask()
{
	if (~player_.tileMask_ & missingCollisionMask_) // if missing mask is not part of mask
	{
		int hitboxFlags = (1 << Player::HitboxRole::Body) | (1 << Player::HitboxRole::Tail);
		if (!(player_.AreHitboxesTouchingInteractTile(hitboxFlags, missingCollisionMask_)))
		{
			player_.AddTileMask(hitboxFlags, missingCollisionMask_);
		}
	}
}