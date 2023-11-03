#include "precomp.h"
#include "Knight.h"

#include "KeyboardManager.h"
#include "Scorekeeper.h"

#include "Shape.h"
#include "Hitbox.h"
#include "TilesetData.h"

#include "EnemyState.h"
#include "KnightWalkingState.h"
#include "KnightRecoilState.h"
#include "KnightAttackingState.h"

#include <iostream>

#include "KnightRewindData.h"


Knight::Knight()
	: Enemy{ObjectClass::Knight, 0, 0, ObjectId::Enemy, ObjectId::Player | ObjectId::PlayerAttack }
{	
	// Set size here, but position will change depending on level.	
	shape_->size_ = float2{ 64, 32 };

	// Hitboxes.
	hitboxCount_ = Knight::HitboxRole::HitboxRoleCount;
	hitboxes_ = new Hitbox[hitboxCount_];

	hitboxes_[Knight::HitboxRole::Head] = Hitbox{ shape_->size_.x };
	hitboxes_[Knight::HitboxRole::Body] = Hitbox{ shape_->size_.x };
	hitboxes_[Knight::HitboxRole::Arm] = Hitbox{ shape_->size_.x };
	hitboxes_[Knight::HitboxRole::ObstacleDetector] = Hitbox{ shape_->size_.x };
	hitboxes_[Knight::HitboxRole::GapDetector] = Hitbox{ shape_->size_.x };
	hitboxes_[Knight::HitboxRole::PlayerDetector] = Hitbox{ shape_->size_.x };

	// The various sprites of the states we can be in. Preload them and keep them ready to swap out on the fly.	
	spriteSheetsCount_ = SpriteSheet::SpriteSheetCount;
	spriteSheets_ = new Sprite * [spriteSheetsCount_];
	spriteSheets_[SpriteSheet::Walking] = new Sprite{ new Surface{"assets/sprites/knight.png"}, true, 64, 32 };
	spriteSheets_[SpriteSheet::Attacking] = new Sprite{ new Surface{"assets/sprites/knight_attack.png"}, true, 64, 32 };
	spriteSheets_[SpriteSheet::Recoiling] = new Sprite{ new Surface{"assets/sprites/knight.png"}, true, 64, 32 };

	// Archer walking speed.
	horizontalAcceleration_ = 300.0f;
}


void Knight::Init(const float2 startTileToCenterOn, Scorekeeper* scorekeeper, const Vector2* playerPosition)
{
	// Reset hp, set positoin, store player position ref.
	Enemy::Init(startTileToCenterOn, playerPosition, scorekeeper);

	// Set initial state.
	state_ = new KnightWalkingState{ *this };
	state_->Enter();

	// Give tile collision a chance to run before moving, otherwise detectors won't know what they are over.
	stateDuration_ = 0.1f;
	currentAction_ = Action::None;

	// Activate.
	isActive_ = true;
}


void Knight::Update(float deltaTime, float gravity)
{
	Enemy::FinishUpdate(deltaTime, gravity);
}


void Knight::StoreRewindData(KnightRewindData& rewindData)
{
	if (isActive_)
	{
		rewindData.isInvulnerable_ = isInvulnerable_;
		rewindData.elapsedInvulnerableTime_ = elapsedInvulnerableTime_;
		rewindData.stateIndex_ = state_->GetStateIndex();

		state_->StoreRewindData(rewindData.enemyStateRewindData_);
	}
	
	Enemy::StoreRewindData(rewindData.enemyRewindData_);
}


void Knight::Rewind(KnightRewindData& rewindData)
{
	sprite_ = spriteSheets_[rewindData.stateIndex_];

	Enemy::Rewind(rewindData.enemyRewindData_);

	SetSpriteFacing(isFlipped_);
	isSpriteBlinking_ = false;
}


void Knight::Resume(KnightRewindData& rewindData)
{
	isInvulnerable_ = rewindData.isInvulnerable_;
	elapsedInvulnerableTime_ = rewindData.elapsedInvulnerableTime_;
	
	ActorState* state{ GetResumedState(rewindData) };

	if (state != nullptr)
	{
		ChangeState(state, true);
	}

	Enemy::Resume(rewindData.enemyRewindData_);
}


ActorState* Knight::GetResumedState(KnightRewindData& rewindData)
{
	switch (rewindData.stateIndex_)
	{
	case Knight::SpriteSheet::Walking:
		return new KnightWalkingState{ *this, rewindData.enemyStateRewindData_ };
	case Knight::SpriteSheet::Attacking:
		return new KnightAttackingState{ *this, rewindData.enemyStateRewindData_ };	
	case Knight::SpriteSheet::Recoiling:
		return new KnightRecoilState{ *this, rewindData.enemyStateRewindData_ };
	default:
		return nullptr;
	}
}


void Knight::ProcessTimers(float deltaTime)
{
	if (isInvulnerable_)
	{
		elapsedInvulnerableTime_ += deltaTime;
		isSpriteBlinking_ = !isSpriteBlinking_;

		if (elapsedInvulnerableTime_ >= maxInvulnerableTime_)
		{
			isSpriteBlinking_ = false;
			isInvulnerable_ = false;
			elapsedInvulnerableTime_ = 0.0f;
		}
	}
}


void Knight::ResolveObstacleCollisions(const float2& contactNormals)
{
	Actor::ResolveObstacleCollisions(contactNormals);
}


void Knight::ResolveObjectCollisions()
{
	// Hit by player attack? Take damage => death.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		if (objectCollisionList_[index]->objectLayer_ == ObjectId::PlayerAttack && !isInvulnerable_)
		{
			// TODO: make composite classes that gameobject could have. each is a pointer to the class. make weapon a composite class with the damage it should deal + other weapony stuff?
			TakeDamage(1);

			if (currentHp_ <= 0)
			{
				scorekeeper_->ChangeScore(Scorekeeper::Score::Other, 100);
				isActive_ = false;
			}

			// If weapon is right of archer, recoil to the left and vice versa.
			float horizontalRecoil{ objectCollisionList_[index]->shape_->center_.x > shape_->center_.x ? -1.0f : 1.0f };
			ChangeState(new KnightRecoilState{ *this, { horizontalRecoil, -1.0f} });
		}
	}


	// Reset object list.
	Actor::ResolveObjectCollisions();
}


Action Knight::GetEnemyAction(float)
{
	// Check if state duration has expired.
	if (stateDuration_ <= 0.0f)
	{
		stateDuration_ = 1.6f;
		currentAction_ = (currentAction_ == Action::Right ? Action::Left : Action::Right);
	}

	return currentAction_;
}


void Knight::Draw(Surface* screen, int x, int y)
{
	if (!isSpriteBlinking_)
	{
		GameObject::Draw(screen, x, y);
	}
	
#ifdef _DEBUG
	Enemy::Draw(screen, x + static_cast<int>(shape_->size_.x * 0.5f), y);

	screen->Box(x, y, x + static_cast<int>(shape_->size_.x - 1), y + static_cast<int>(shape_->size_.y - 1), 0xffffff);

	for (int i{ 0 }; i < Knight::HitboxRole::HitboxRoleCount; ++i)
	{
		screen->Box(x + hitboxes_[i].offset_.x, y + hitboxes_[i].offset_.y, x + hitboxes_[i].offset_.x + hitboxes_[i].size_.x - 1, y + hitboxes_[i].offset_.y + hitboxes_[i].size_.y - 1, 0xffff00);
	}
#endif // _DEBUG
}