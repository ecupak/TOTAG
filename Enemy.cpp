#include "precomp.h"
#include "Enemy.h"

#include "KeyboardManager.h"
#include "EnemyState.h"
#include "Shape.h"

#include "EnemyRewindData.h"


Enemy::Enemy(const ObjectClass objectClass, int tileLayer, int tileMask, int objectLayer, int objectMask)
	: Actor{ objectClass, tileLayer, tileMask, objectLayer, objectMask }
	, currentAction_{ Action::None }
{	}


Enemy::~Enemy() {}


void Enemy::Init(const float2 startTileToCenterOn, const Vector2* playerPosition, Scorekeeper* scorekeeper)
{
	// Adjust start so the object's sprite is horizontally centered in the intended tile and its bottom is aligned with the bottom of the tile.
	float2 offset{
		(shape_->size_.x - TILE_WIDTH) * 0.5f,
		(shape_->size_.y - TILE_HEIGHT)
	};

	float2 enemyPosition{
		startTileToCenterOn.x - offset.x,
		startTileToCenterOn.y - offset.y
	};

	shape_->start_ = enemyPosition;

	playerPosition_ = playerPosition;
	
	currentHp_ = maxHp_;

	scorekeeper_ = scorekeeper;
}


void Enemy::StoreRewindData(EnemyRewindData* rewindData)
{
	if (isActive_)
	{
		rewindData->stateDuration_ = stateDuration_;
		rewindData->currentAction_ = currentAction_;
	}

	Actor::StoreRewindData(rewindData->actorRewindData_);
}


void Enemy::Rewind(EnemyRewindData* rewindData)
{
	Actor::Rewind(rewindData->actorRewindData_);
}


void Enemy::Resume(EnemyRewindData* rewindData)
{
	stateDuration_ = rewindData->stateDuration_;
	currentAction_ = static_cast<Action>(rewindData->currentAction_);

	Actor::Resume(rewindData->actorRewindData_);
}


void Enemy::FinishUpdate(float deltaTime, float gravity)
{
	// Do nothing if not onstage.
	if (!isOnstage_)
	{
		return;
	}

	// Run state with enemy-generated action.
	command_ = nullptr;
	ActorState* state{ state_->HandleInput(GetEnemyAction(deltaTime), command_) };

	if (state != nullptr)
	{
		ChangeState(state);
	}

	// Run command, update velocity.
	Actor::FinishUpdate(deltaTime, gravity);
}


void Enemy::Draw(Surface* screen, int x, int y)
{
#ifdef _DEBUG
	screen->Print(state_->name_, x, y - 16, 0xAA00AA);
#else
	screen;
	x;
	y;
#endif
}