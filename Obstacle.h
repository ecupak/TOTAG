#pragma once

#include "GameObject.h"


// The tiles around the map that act as floors and walls.
class Obstacle : public GameObject	
{
public:
	Obstacle(const float2 position)
		: GameObject{ CollisionID::Obstacle, CollisionID::Player | CollisionID::Camera }
	{	
		shape_.size_ = float2{ 16.0f, 16.0f };
		shape_.SetPosition(position);

		sprite_ = new Sprite{ new Surface{"assets/obstacle.png"}, true, 1 };
	}
	

	void CollidedWith(GameObject* go) override
	{
		return;
		if (go->tileLayer_ & CollisionID::Player)
		{
			// First make sure we're still colliding (could have been moved out by another obstacle).
			bool hasOverlap{
				!(go->shape_.offset_.x > shape_.end_.x || go->shape_.end_.x < shape_.offset_.x)
				&& !(go->shape_.offset_.y > shape_.end_.y || go->shape_.end_.y < shape_.offset_.y) 
			};

			if (hasOverlap)
			{
				// go->shape_.MoveCenter(EjectForeignObject(go));
			}
		}
	}
};