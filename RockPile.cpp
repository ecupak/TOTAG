#include "precomp.h"
#include "RockPile.h"

#include "Shape.h"
#include "Hitbox.h"
#include "Subject.h"
#include "Event.h"

#include "RockPileRewindData.h"


RockPile::RockPile()
	: GameObject{ ObjectClass::RockPile, ObjectType::Prop, 0, 0, ObjectId::Collectible, ObjectId::Player }
	, onCollect_{ new Subject{} }
{	

	shape_->size_ = float2{ 16, 16 };

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 6, 7, 4, 9, Hitbox::Mode::Object };
}


RockPile::~RockPile()
{
	delete onCollect_;
}


void RockPile::Init(const float2 startTileToCenterOn, Sprite* sprite)
{
	shape_->SetPosition(startTileToCenterOn);

	sprite_ = sprite;

	isActive_ = true;
}


void RockPile::Update(RockPileRewindData& rewindData)
{
	StoreRewindData(rewindData);
}


void RockPile::StoreRewindData(RockPileRewindData& rewindData)
{
	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void RockPile::Rewind(RockPileRewindData& rewindData)
{
	GameObject::Rewind(rewindData.gameObjectRewindData_);
}


void RockPile::Resume(RockPileRewindData& rewindData)
{
	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void RockPile::OnObjectCollision(GameObject*)
{
	onCollect_->Notify(ObjectClass::RockPile, Event::OnTouch);
	isActive_ = false;
}