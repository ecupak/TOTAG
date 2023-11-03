#include "precomp.h"
#include "SavePoint.h"

#include "Subject.h"

#include "Shape.h"
#include "Hitbox.h"
#include "Subject.h"
#include "Event.h"

#include "SavePointRewindData.h"


// SavePointParticles* SavePoint::particles_{ new SavePointParticles };

SavePoint::SavePoint()
	: GameObject{ ObjectClass::SavePoint, ObjectType::Prop, 0, 0, ObjectId::Trigger, ObjectId::Player }
	, onPlayerTouch_{ new Subject{} }
{
	shape_->size_ = float2{ 16, 32 };


	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 0, 0, 16, 16, Hitbox::Mode::Object | Hitbox::Mode::Trigger };
}


SavePoint::~SavePoint()
{
	//delete particles_;
	//particles_ = nullptr;

	delete onPlayerTouch_;
}


void SavePoint::Init(const float2 startTileToCenterOn, Sprite* sprite)
{
	shape_->SetPosition({ startTileToCenterOn.x, startTileToCenterOn.y - 16.0f });

	sprite_ = sprite;

	isActive_ = true;
}


void SavePoint::StoreRewindData(SavePointRewindData& rewindData)
{
	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void SavePoint::Rewind(SavePointRewindData& rewindData)
{
	GameObject::Rewind(rewindData.gameObjectRewindData_);
}


void SavePoint::Resume(SavePointRewindData& rewindData)
{
	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void SavePoint::OnObjectCollision(GameObject* object)
{
	if (object->objectClass_ == ObjectClass::Player) 
	{
		onPlayerTouch_->Notify(objectClass_, Event::OnTouch);

		isActive_ = false;
	}
}


void SavePoint::Draw(Surface* screen, int x, int y)
{
	GameObject::Draw(screen, x, y);

#ifdef _DEBUG
	screen->Box(x + hitboxes_[0].offset_.x, y + hitboxes_[0].offset_.y, x + hitboxes_[0].offset_.x + hitboxes_[0].size_.x - 1, y + hitboxes_[0].offset_.y + hitboxes_[0].size_.y - 1, 0xffff00);
#endif // _DEBUG
}