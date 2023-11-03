#include "precomp.h"
#include "LevelExit.h"

#include "Subject.h"

#include "Shape.h"
#include "Hitbox.h"

#include "Event.h"


LevelExit::LevelExit()
	: GameObject{ ObjectClass::Exit, ObjectType::Prop, 0, 0, ObjectId::Trigger, ObjectId::Player }
{	
	shape_->size_ = float2{ 16, 16 };

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 0, 0, 16, 16, Hitbox::Mode::Object | Hitbox::Mode::Trigger };
}


void LevelExit::Place(const float2 startTileToCenterOn)
{
	shape_->SetPosition(startTileToCenterOn);

	isActive_ = true;
}


void LevelExit::Draw(Surface*, int, int) {}