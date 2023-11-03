#include "precomp.h"
#include "Coin.h"

#include "Scorekeeper.h"
#include "Subject.h"

#include "Shape.h"
#include "Hitbox.h"
#include "Subject.h"
#include "Event.h"

#include "CoinRewindData.h"
#include "GameObjectRewindData.h"

#include <iostream>


Coin::Coin()
	: GameObject{ObjectClass::Coin, ObjectType::Prop, 0, 0, ObjectId::Collectible, ObjectId::Player }
	, onCollect_{ new Subject{} }
{	
	shape_->size_ = float2{ 16, 16 };

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 5, 3, 6, 10, Hitbox::Mode::Object };
}


Coin::~Coin()
{
	delete onCollect_;
}


void Coin::Init(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper)
{
	shape_->SetPosition(startTileToCenterOn);

	sprite_ = sprite;

	scorekeeper_ = scorekeeper;

	isActive_ = true;
}


void Coin::StoreRewindData(CoinRewindData& rewindData)
{
	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void Coin::Rewind(CoinRewindData& rewindData)
{	
	GameObject::Rewind(rewindData.gameObjectRewindData_);
}


void Coin::Resume(CoinRewindData& rewindData)
{
	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void Coin::OnObjectCollision(GameObject*)
{
	scorekeeper_->ChangeScore(Scorekeeper::Score::Other, 250);
	onCollect_->Notify(objectClass_, Event::OnTouch);
	isActive_ = false;
}
