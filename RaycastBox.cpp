#include "precomp.h"
#include "RaycastBox.h"

#include "PlayerBouncingState.h"

#include "Raycaster.h"
#include "Vector2.h"
#include "Level.h"


RaycastBox::RaycastBox()
	: GameObject{ CollisionID::Player, CollisionID::Obstacle }
{
	raycaster_ = new Raycaster{};

	name_ = "Raycast Box";
}


RaycastBox::~RaycastBox()
{
	delete raycaster_;
}


// Probably remove raycaster from this box and give both as equal siblings to player. Box can collect objects, and player can pass that to raycaster.
Vector2 RaycastBox::ResolveTileCollisions(GameObject* focus, GameObject objects[], int& objectCount, int touchedObjectsIndex[])
{
	// Store object distance data.
	const int resultMaxCount{ 50 };
	SortData results_[resultMaxCount]{};
	int resultsCurrentIndex_{ 0 };


	// Get distance from raycast source.
	Vector2 playerCenter{ focus->shape_.center_ };
	for (int index{ 0 }; index < objectCount; ++index)
	{		
		float xDiff = objects[index].shape_.center_.x - playerCenter.x;
		float yDiff = objects[index].shape_.center_.y - playerCenter.y;
				
		results_[resultsCurrentIndex_] = {index, (xDiff * xDiff) + (yDiff * yDiff)};
		++resultsCurrentIndex_;

		// If we get too many objects to compare to, bail and work with what we've collected. This *will* most likely cause an issue in the game.
		if (resultsCurrentIndex_ == resultMaxCount)
		{
			break;
		}
	}


	// Bubble sort based on distance to raycast source. Closest obstacles should be resolved first.
	SortData temp{ 0, 0.0f };
	for (int position{ 0 }; position < resultsCurrentIndex_ - 1; ++position)
	{
		for (int compareTo{ position + 1 }; compareTo < resultsCurrentIndex_; ++compareTo)
		{
			if (results_[position].distanceAway > results_[compareTo].distanceAway)
			{
				temp = results_[position];
				results_[position] = results_[compareTo];
				results_[compareTo] = temp;
			}
		}
	}

	
	// Resolve collisions in order of closest to furthest objects. Report back the first 4 objects collided with (should never be more than that collided with at once).
	Vector2 contactNormal{ 0.0f, 0.0f };
	Vector2 contactNormals{ 0.0f, 0.0f };
	float timeToContact{ 0.0f };	
	objectCount = 0; // Reuse as out variable.

	/*
	for (int index{ 0 }; index < resultsCurrentIndex_; ++index)
	{
		if (raycaster_->DynamicRaycastToObject(focus, objects[results_[index].objectIndex],
			contactNormal, timeToContact) && timeToContact < 1.0f
			)
		{
			// We collided with this object. Collect normals and adjust velocity.
			contactNormals += contactNormal;
			focus->velocity_ += contactNormal * focus->velocity_.Absolute() * (1.0f - timeToContact);
			
			// Store in order of collision.
			touchedObjectsIndex[objectCount] = results_[index].objectIndex;
			++objectCount;
		}
	}
	*/

	return contactNormals;
}