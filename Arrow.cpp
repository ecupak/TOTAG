// [Credit] Use in arrow pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "Arrow.h"

#include "Hitbox.h"
#include "Shape.h"
#include "TileCollision.h"
#include "Vector2.h"

#include "ArrowRewindData.h"


Sprite* Arrow::arrowSprite_{ new Sprite{ new Surface{"assets/sprites/arrow_tip.png"}, true, 1 } };


Arrow::Arrow()
	: GameObject{ ObjectClass::Arrow, ObjectType::Prop,
		0, TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		ObjectId::EnemyAttack, ObjectId::Player }
	, prevPosition_{ new Vector2{} }
{
	shape_->size_ = float2{ 1, 1 };

	sprite_ = arrowSprite_;

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ 1, 0, 0, 1, 1, Hitbox::Mode::Obstacle | Hitbox::Mode::Object };
	hitboxes_[0].tileMask_ = tileMask_;
	
	friction_ = 10.0f;
}


Arrow::~Arrow()
{
	delete prevPosition_;

	delete arrowSprite_;
	arrowSprite_ = nullptr;
}


void Arrow::Init(const float2& position, const int archerWidth, const float2& direction, const float speed)
{
	SetArrowStartingPosition(position, archerWidth, direction);

	*velocity_ = direction * speed;

	isStuck_ = false;
	hitboxes_[0].mode_ = Hitbox::Mode::Obstacle | Hitbox::Mode::Object;

	isMarkedForDeactivation_ = false;
	lifetime_ = 10.0f;
	isActive_ = true;
}


void Arrow::Update(float deltaTime, float gravity)
{
	lifetime_ -= deltaTime;
	isActive_ = lifetime_ > 0.0f && !isMarkedForDeactivation_;

	if (!isStuck_)
	{
		velocity_->y += 0.25f * gravity * deltaTime;

		*frameVelocity_ = *velocity_ * deltaTime;

		ApplyFriction(deltaTime);
	}
}


void Arrow::StoreRewindData(ArrowRewindData& rewindData)
{
	if (isActive_)
	{
		rewindData.isStuck_ = isStuck_;
		rewindData.lifetime_ = lifetime_;
		rewindData.previousPosition_ = static_cast<float2>(*prevPosition_);
	}

	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void Arrow::Rewind(ArrowRewindData& rewindData)
{
	*prevPosition_ = rewindData.previousPosition_;

	GameObject::Rewind(rewindData.gameObjectRewindData_);

	// No sprite, image must be calculated.
	CalculateArrowEnd();
}


void Arrow::Resume(ArrowRewindData& rewindData)
{
	isStuck_ = rewindData.isStuck_;
	lifetime_ = rewindData.lifetime_;

	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void Arrow::ResolveObstacleCollisions(const float2& contactNormals)
{
	// Once stuck in a surface, no longer moves by gravity.
	if (isStuck_)
	{
		return;
	}

	// Arrows do not slide along surfaces.
	// Find the normal that did not hit a surface (is zero) and set corresponding frameVelocity to 0.
	if (contactNormals.x != 0.0f || contactNormals.y != 0.0f)
	{
		isStuck_ = true;
		hitboxes_[0].mode_ = Hitbox::Mode::None;

		if (contactNormals.x == 0.0f)
		{
			frameVelocity_->x = 0.0f;
		}
		else
		{
			frameVelocity_->y = 0.0f;
		}
	}

	// Update position and get new arrow end.
	*prevPosition_ = shape_->start_;

	shape_->MovePosition(static_cast<float2>(*frameVelocity_));
	shape_->Truncate(static_cast<float2>(contactNormals));
	
	CalculateArrowEnd();
}


void Arrow::OnObjectCollision(GameObject*)
{
	isMarkedForDeactivation_ = true;
}


void Arrow::Draw(Surface* screen, int x, int y)
{
	float2 pos{ static_cast<float>(x), static_cast<float>(y) };

	screen->Line(pos.x, pos.y, pos.x + shaftEnd_.x, pos.y + shaftEnd_.y, shaftColor);
	screen->Line(pos.x, pos.y, pos.x + tipEnd_.x, pos.y + tipEnd_.y, tipColor);

#ifdef _DEBUG
	screen->Box(x + hitboxes_[0].offset_.x, y + hitboxes_[0].offset_.y, x + hitboxes_[0].offset_.x + hitboxes_[0].size_.x - 1, y + hitboxes_[0].offset_.y + hitboxes_[0].size_.y - 1, 0xffff00);
#endif // _DEBUG
}


// Arrow point is placed on a circle surrounding the start position.
void Arrow::SetArrowStartingPosition(const float2& position, const int archerWidth, const float2& direction)
{
	// Radius is arrow length + half body.
	float radius{ 1.0f * shaftLength_ + (archerWidth >> 1) };
	
	// Normalized vector components are the same value as the result of passing the angle of the vector to cos & sin.
	// Position of arrow tip around archer.
	shape_->SetPosition({
		position.x + (direction.x * radius),
		position.y + (direction.y * radius)
	});

	// Set previous position to center of archer.
	*prevPosition_ = position;
	CalculateArrowEnd();
}


void Arrow::CalculateArrowEnd()
{
	// Don't calculate position when stuck because prev position will be same as start (didn't move last frame).
	if (isStuck_)
	{
		return;
	}

	// Get unit vector to prev position.
	float2 directionToPrevPosition{ normalize(static_cast<float2>(*prevPosition_ - shape_->start_)) };

	// Get tip length and shaft lenght.
	tipEnd_ = directionToPrevPosition * static_cast<float>(tipLength_);
	shaftEnd_ = directionToPrevPosition * static_cast<float>(shaftLength_);
}