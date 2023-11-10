#include "precomp.h"
#include "Soul.h"

#include "Scorekeeper.h"
#include "Subject.h"

#include "Shape.h"
#include "Hitbox.h"
#include "Subject.h"
#include "Event.h"

#include "SoulRewindData.h"


Soul::Soul()
	: GameObject{ObjectClass::Soul, ObjectType::Prop, 0, 0, ObjectId::Collectible, ObjectId::Player }
	, onCollect_{ new Subject{} }
{	
	shape_->size_ = float2{ 16, 16 };

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 5, 3, 6, 10, Hitbox::Mode::Object };
}


Soul::~Soul()
{
	delete onCollect_;
}


void Soul::Init(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper)
{
	shape_->SetPosition(startTileToCenterOn);

	sprite_ = sprite;

	scorekeeper_ = scorekeeper;

	isActive_ = true;
}


// Store rewind data only because it is used to create the save data.
void Soul::StoreRewindData(SoulRewindData& rewindData)
{
	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


// Soul coins do not return once collected. But rewind/resume is used when loading a save file.
void Soul::Rewind(SoulRewindData& rewindData)
{
	GameObject::Rewind(rewindData.gameObjectRewindData_);
}


void Soul::Resume(SoulRewindData& rewindData)
{
	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void Soul::OnObjectCollision(GameObject*)
{
	scorekeeper_->ChangeScore(Scorekeeper::Score::Soul, 1000);
	onCollect_->Notify(objectClass_, Event::OnTouch);
	isActive_ = false;
}
