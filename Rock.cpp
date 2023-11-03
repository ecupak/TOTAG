// [Credit] Use in rock pool: https://gameprogrammingpatterns.com/object-pool.html
#include "precomp.h"
#include "Rock.h"

#include "Hitbox.h"
#include "Shape.h"
#include "TileCollision.h"

#include "RockRewindData.h"


Rock::Rock()
	: GameObject{ ObjectClass::Rock, ObjectType::Prop, 
		0, TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline,
		ObjectId::PlayerAttack, ObjectId::Enemy }
{	
	shape_->size_ = float2{ 4, 4 };

	hitboxCount_ = 1;
	hitboxes_ = new Hitbox[hitboxCount_];
	hitboxes_[0] = Hitbox{ shape_->size_.x, 0, 0, static_cast<int>(shape_->size_.x), static_cast<int>(shape_->size_.y), Hitbox::Mode::Obstacle | Hitbox::Mode::Object };
	hitboxes_[0].tileMask_ = TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge | TileCollisionID::Trampoline;
}


void Rock::Init(const float2 position, const float2 velocity, Sprite* sprite)
{
	shape_->SetPosition(position);
	*velocity_ = velocity;
	lifetime_ = 3.0f;
	
	sprite_ = sprite;

	friction_ = 100.0f;

	hitboxes_[0].mode_ = Hitbox::Mode::Obstacle | Hitbox::Mode::Object;

	isActive_ = true;
}


void Rock::Update(float deltaTime, float gravity)
{
	lifetime_ -= deltaTime;
	isActive_ = lifetime_ > 0.0f;

	velocity_->y += gravity * deltaTime;
	
	*frameVelocity_ = *velocity_ * deltaTime;

	ApplyFriction(deltaTime);
}


void Rock::StoreRewindData(RockRewindData& rewindData)
{
	if (isActive_)
	{
		rewindData.lifetime_ = lifetime_;
	}

	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void Rock::Rewind(RockRewindData& rewindData)
{
	GameObject::Rewind(rewindData.gameObjectRewindData_);
}


void Rock::Resume(RockRewindData& rewindData)
{
	lifetime_ = rewindData.lifetime_;

	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void Rock::ResolveObstacleCollisions(const float2& contactNormals)
{
	shape_->MovePosition(static_cast<float2>(*frameVelocity_));
	shape_->Truncate(static_cast<float2>(contactNormals));
	
	// Hitting the ground increases friction.
	if (contactNormals.y == -1.0f)
	{
		friction_ = 700.0f;
	}

	// Apply vector reflection when hitting a surface.
	if (contactNormals.x != 0.0f || contactNormals.y != 0.0f)
	{
		Bounce(contactNormals, 0.6f);
	}
}


void Rock::OnObjectCollision(GameObject* object)
{
	float2 contactNormals
	{
		shape_->center_.x > object->shape_->center_.x ? 1.0f : -1.0f,
		0.0f
	};

	Bounce(contactNormals, 0.4f);
}


void Rock::Bounce(const float2 contactNormals, const float elasticity)
{
	// The rock becomes harmless(remove object mode).
	hitboxes_[0].mode_ = Hitbox::Mode::Obstacle;

	// [Credit] http://www.sunshine2k.de/articles/coding/vectorreflection/vectorreflection.html
	// [Credit] Also got help from Remi, making me realize I didn't normalize the contact normals.
	float2 normalizedContactNormals = normalize(contactNormals);
	Vector2 reflection = *velocity_ - (2.0f * dot(static_cast<float2>(*velocity_), normalizedContactNormals) * normalizedContactNormals);

	*velocity_ = reflection * elasticity;
}