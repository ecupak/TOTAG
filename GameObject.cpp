#include "precomp.h"
#include "GameObject.h"

#include "Shape.h"
#include "Hitbox.h"
#include "Shape.h"
#include "Vector2.h"

#include "GameObjectRewindData.h"

#include <iostream>


// Use bitwise | (or) to combine the CollisionID values that make up the entire layer or mask.
GameObject::GameObject(const ObjectClass objectClass, const int objectType, int tileLayer, int tileMask, int objectLayer, int objectMask)
	: objectClass_{ objectClass }
	, objectType_{ objectType }
	, tileLayer_{ tileLayer }
	, tileMask_{ tileMask }
	, objectLayer_{ objectLayer }
	, objectMask_{ objectType == ObjectType::Actor ? objectMask : ObjectId::None }
	, shape_{ new Shape{} }
	, velocity_{ new Vector2{} }
	, frameVelocity_{ new Vector2{} }
{	}


GameObject::~GameObject()
{
	delete shape_;
	delete velocity_;
	delete frameVelocity_;

	delete[] hitboxes_;
}


void GameObject::StoreRewindData(GameObjectRewindData* rewindData)
{
	rewindData->isActive_ = isActive_;

	// Only store the rest if active
	if (isActive_)
	{
		rewindData->isActive_ = isActive_;
		rewindData->isFlipped_ = isFlipped_;
		rewindData->isOnstage_ = isOnstage_;

		rewindData->overlappingTileLayers_ = overlappingTileLayers_;

		if (sprite_ != nullptr)
		{
			rewindData->spriteFrame_ = sprite_->GetFrame();
		}

		rewindData->position_ = static_cast<float2>(shape_->start_);
		rewindData->velocity_ = static_cast<float2>(*velocity_);
	}
}


void GameObject::Rewind(GameObjectRewindData* rewindData)
{
	isActive_ = rewindData->isActive_;
	isFlipped_ = rewindData->isFlipped_;
	isOnstage_ = rewindData->isOnstage_;

	if (sprite_ != nullptr)
	{
		sprite_->SetFrame(rewindData->spriteFrame_);
	}

	shape_->SetPosition(static_cast<float2>(rewindData->position_));
}


void GameObject::Resume(GameObjectRewindData* rewindData)
{
	overlappingTileLayers_ = rewindData->overlappingTileLayers_;
	*velocity_ = static_cast<float2>(rewindData->velocity_);
}


// Do not update if not onstage. If onstage, update, but turn off onstage. AABB collision manager will turn it back on if object is within camera extended bounds.
void GameObject::Update(float, float) 
{
	if (!isOnstage_)
	{
		return;
	}
}


void GameObject::Draw(Surface* screen, int x, int y)
{
	sprite_->Draw(screen, x, y);
}


// Apply horizontal friction (move towards zero).
void GameObject::ApplyFriction(float deltaTime)
{
	if (velocity_->x != 0.0f)
	{
		velocity_->x = (velocity_->x > 0.0f)
			? max(0.0f, velocity_->x - (friction_ * deltaTime))
			: min(0.0f, velocity_->x + (friction_ * deltaTime));
	}
}


void GameObject::PrepareSprite() {}
void GameObject::ResolveObstacleCollisions(const float2&) {}
void GameObject::OnObjectCollision(GameObject*) {}
void GameObject::ResolveObjectCollisions() {}