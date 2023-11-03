#include "precomp.h"
#include "Archer.h"

#include "KeyboardManager.h"
#include "Scorekeeper.h"
#include "ArrowPool.h"

#include "Shape.h"
#include "Hitbox.h"
#include "TilesetData.h"

#include "EnemyState.h"
#include "ArcherStandingState.h"
#include "ArcherAlertState.h"
#include "ArcherRecoilingState.h"
#include "ArcherShootingState.h"
#include "ArcherWalkingState.h"

#include "ArcherRewindData.h"


// Pool bulk creation during array initialization.
Archer::Archer()
	: Enemy{ObjectClass::Archer, 0, 0, ObjectId::Enemy, ObjectId::Player | ObjectId::PlayerAttack }
{	
	// Set size here, but position will change depending on level.	
	shape_->size_ = float2{ 40, 24 };

	// Hitboxes.
	hitboxCount_ = Archer::HitboxRole::HitboxRoleCount;
	hitboxes_ = new Hitbox[hitboxCount_];

	hitboxes_[Archer::HitboxRole::Head] = Hitbox{ shape_->size_.x };
	hitboxes_[Archer::HitboxRole::Body] = Hitbox{ shape_->size_.x };
	hitboxes_[Archer::HitboxRole::PersonalSpace] = Hitbox{ shape_->size_.x };
	hitboxes_[Archer::HitboxRole::ObstacleDetector] = Hitbox{ shape_->size_.x };
	hitboxes_[Archer::HitboxRole::GapDetector] = Hitbox{ shape_->size_.x };
	hitboxes_[Archer::HitboxRole::PlayerDetector] = Hitbox{ shape_->size_.x };

	// The various sprites of the states we can be in. Preload them and keep them ready to swap out on the fly.	
	spriteSheetsCount_ = SpriteSheet::SpriteSheetCount;
	spriteSheets_ = new Sprite * [spriteSheetsCount_];
	spriteSheets_[SpriteSheet::Standing] = new Sprite{ new Surface{"assets/sprites/archer.png"}, true, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y) };
	spriteSheets_[SpriteSheet::Walking] = new Sprite{ new Surface{"assets/sprites/archer.png"}, true, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y) };
	spriteSheets_[SpriteSheet::Alert] = new Sprite{ new Surface{"assets/sprites/archer.png"}, true, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y) };
	spriteSheets_[SpriteSheet::Shooting] = new Sprite{ new Surface{"assets/sprites/archer_attack.png"}, true, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y) };
	spriteSheets_[SpriteSheet::Recoiling] = new Sprite{ new Surface{"assets/sprites/archer.png"}, true, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y) };

	// Archer walking speed.
	horizontalAcceleration_ = 210.0f;

	// Set max hp.
	maxHp_ = 2;
}


// Creation by pool for active object.
void Archer::Init(const float2 startTileToCenterOn, Sprite* lockonSprite, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool)
{
	// Set position, store ref to player position, reset HP.
	Enemy::Init(startTileToCenterOn, playerPosition, scorekeeper);

	// Aiming sprite indicator.
	lockonSprite_ = lockonSprite;

	// Store ammo ref.
	arrowPool_ = arrowPool;

	// Give tile collision a chance to run before moving, otherwise detectors won't know what they are over.
	stateDuration_ = 0.1f;
	currentAction_ = Action::None;

	// Set initial state.
	state_ = new ArcherStandingState{ *this };
	state_->Enter();

	// Activate.
	isActive_ = true;
}


void Archer::Update(float deltaTime, float gravity)
{
	Enemy::FinishUpdate(deltaTime, gravity);
}


void Archer::StoreRewindData(ArcherRewindData& rewindData)
{
	if (isActive_)
	{
		rewindData.attackDelay_ = attackDelay_;
		rewindData.lockonSpriteFrame_ = lockonSpriteFrame_;
		rewindData.stateIndex_ = state_->GetStateIndex();
	
		state_->StoreRewindData(rewindData.enemyStateRewindData_);
	}

	Enemy::StoreRewindData(rewindData.enemyRewindData_);
}


void Archer::Rewind(ArcherRewindData& rewindData)
{
	lockonSpriteFrame_ = rewindData.lockonSpriteFrame_;
	sprite_ = spriteSheets_[rewindData.stateIndex_];

	Enemy::Rewind(rewindData.enemyRewindData_);

	SetSpriteFacing(isFlipped_);
}


void Archer::Resume(ArcherRewindData& rewindData)
{
	attackDelay_ = rewindData.attackDelay_;

	ActorState* state{ GetResumedState(rewindData) };
	
	if (state != nullptr)
	{
		ChangeState(state, true);
	}

	Enemy::Resume(rewindData.enemyRewindData_);
}


ActorState* Archer::GetResumedState(ArcherRewindData& rewindData)
{
	switch (rewindData.stateIndex_)
	{
	case Archer::SpriteSheet::Alert:
		return new ArcherAlertState{ *this, rewindData.enemyStateRewindData_ };
	case Archer::SpriteSheet::Recoiling:
		return new ArcherRecoilingState{ *this, rewindData.enemyStateRewindData_ };
	case Archer::SpriteSheet::Shooting:
		return new ArcherShootingState{ *this, rewindData.enemyStateRewindData_ };
	case Archer::SpriteSheet::Standing:
		return new ArcherStandingState{ *this };
	case Archer::SpriteSheet::Walking:
		return new ArcherWalkingState{ *this, rewindData.enemyStateRewindData_ };
	default:
		return nullptr;
	}
}


void Archer::ResolveObstacleCollisions(const float2& contactNormals)
{
	Actor::ResolveObstacleCollisions(contactNormals);
}


void Archer::ResolveObjectCollisions()
{
	// Hit by player attack? Take damage => death.
	for (int index{ 0 }; index < objectCollisionCount_; ++index)
	{
		if (objectCollisionList_[index]->objectLayer_ == ObjectId::PlayerAttack)
		{
			// TODO: make composite classes that gameobject could have. each is a pointer to the class. make weapon a composite class with the damage it should deal + other weapony stuff?
			TakeDamage(1);

			if (currentHp_ <= 0)
			{
				scorekeeper_->ChangeScore(Scorekeeper::Score::Other, 125);
				isActive_ = false;
			}

			// If weapon is right of archer, recoil to the left and vice versa.
			float horizontalRecoil{ objectCollisionList_[index]->shape_->center_.x > shape_->center_.x ? -1.0f : 1.0f };
			ChangeState(new ArcherRecoilingState{ *this, { horizontalRecoil, -1.0f} });
		}
	}


	// Reset object list.
	Actor::ResolveObjectCollisions();
}


void Archer::ProcessTimers(float deltaTime)
{
	if (attackDelay_ > 0.0f)
	{
		attackDelay_ -= deltaTime;

	}
}


Action Archer::GetEnemyAction(float)
{
	// Check if state duration has expired.
	if (stateDuration_ <= 0.0f)
	{
		// Select new state.
		switch (RandomUInt() % 3)
		{
		case 0:
			currentAction_ = Action::Left;
			stateDuration_ = 1.0f + (RandomFloat() * 4.0f);
			break;
		case 1:
			currentAction_ = Action::Right;
			stateDuration_ = 1.0f + (RandomFloat() * 4.0f);
			break;
		default:
			currentAction_ = Action::None;
			stateDuration_ = 1.0f + (RandomFloat() * 2.0f);
			break;
		}
	}

	return currentAction_;
}


void Archer::Draw(Surface* screen, int x, int y)
{
	if (lockonSpriteFrame_ >= 0)
	{
		lockonSprite_->SetFrame(lockonSpriteFrame_);

		int2 offset{ x - (lockonSprite_->GetWidth() >> 1), y - (lockonSprite_->GetHeight() >> 1) };

		lockonSprite_->Draw(screen,
			offset.x + hitboxes_[0].offset_.x + (hitboxes_[0].size_.x >> 1),
			offset.y - (hitboxes_[0].offset_.y >> 1)
		);
	}

	GameObject::Draw(screen, x, y);
	
	
#ifdef _DEBUG
	Enemy::Draw(screen, x + static_cast<int>(shape_->size_.x * 0.5f), y);

	screen->Box(x, y, static_cast<int>(x + shape_->size_.x - 1), static_cast<int>(y + shape_->size_.y - 1), 0xffffff);

	for (int i{ 0 }; i < Archer::HitboxRole::HitboxRoleCount; ++i)
	{
		screen->Box(x + hitboxes_[i].offset_.x, y + hitboxes_[i].offset_.y, x + hitboxes_[i].offset_.x + hitboxes_[i].size_.x - 1, y + hitboxes_[i].offset_.y + hitboxes_[i].size_.y - 1, 0xffff00);
	}
#endif // _DEBUG
}