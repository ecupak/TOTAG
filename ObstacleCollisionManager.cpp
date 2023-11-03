#include "precomp.h"
#include "ObstacleCollisionManager.h"

#include "CollisionFinder.h"
#include "Raycaster.h"
#include "GameObject.h"
#include "Hitbox.h"
#include "Shape.h"

#include "MapData.h"
#include "TilesetData.h"

#include "TileCollision.h"


// Used to set array sizes without a magic number.
constexpr int collisionListMaxSize_{ 50 };


ObstacleCollisionManager::ObstacleCollisionManager(const int2& tileSize, const MapData& mapData, GameObject* const activeList[])
	: mapData_{ mapData }
	, activeList_{ activeList }
	, collisionList_{ new const MapTile*[collisionListMaxSize_] }
	, collisionFinder_{ new CollisionFinder{ tileSize, mapData_ } }
	, raycaster_{ new Raycaster{} }
	, tileModes_{ Hitbox::Mode::Obstacle }
{	}


ObstacleCollisionManager::~ObstacleCollisionManager()
{
	delete[] collisionList_;
	delete collisionFinder_;
	delete raycaster_;
}


void ObstacleCollisionManager::ProcessCollisions(const int activeCount)
{
	for (int index{ 0 }; index < activeCount; ++index)
	{
		GameObject* const focus{ activeList_[index] };

		// Actors and props (arrows, rocks) react to walls if they have a tile mask.
		if (focus->isOnstage_ && focus->tileMask_ > 0)
		{
			collisionFinder_->FindCollisionsAgainstFocus(focus, collisionList_, collisionListCurrentIndex_);

			SortObjectsByDistance(focus);

			CheckObjectsAgainstFocus(focus);
		}
	}
}


void ObstacleCollisionManager::SortObjectsByDistance(GameObject* const focus)
{
	// If the list contains none or only 1 item, no need to sort.
	if (collisionListCurrentIndex_ < 2)
	{
		return;
	}

	struct SortHelper
	{
		int objectIndex;
		float distance;
	};

	// Safe because source is also limited by this amount.
	SortHelper results[collisionListMaxSize_]{};

	// Calculate and store distances from tile obstacles to focus.
	for (int index{ 0 }; index < collisionListCurrentIndex_; ++index)
	{
		float xDiff = (collisionList_[index]->position.x + (TILE_WIDTH >> 1)) - focus->shape_->center_.x;
		float yDiff = (collisionList_[index]->position.y + (TILE_HEIGHT >> 1)) - focus->shape_->center_.y;

		results[index] = { index, (xDiff * xDiff) + (yDiff * yDiff) };
	}

	// Bubble sort based on distance to focus.
	SortHelper tempSH;
	const MapTile* tempMT;
	for (int position{ 0 }; position < collisionListCurrentIndex_ - 1; ++position)
	{
		for (int compareTo{ position + 1 }; compareTo < collisionListCurrentIndex_; ++compareTo)
		{
			if (results[position].distance > results[compareTo].distance)
			{
				// Swap the results.
				tempSH = results[position];
				results[position] = results[compareTo];
				results[compareTo] = tempSH;

				// Swap the data.
				tempMT = collisionList_[position];
				collisionList_[position] = collisionList_[compareTo];
				collisionList_[compareTo] = tempMT;
			}
		}
	}
}


// Cascading collision checks.
void ObstacleCollisionManager::CheckObjectsAgainstFocus(GameObject* const focus)
{
	// Data to be returned to the focus.
	Vector2 contactNormals{ 0.0f, 0.0f };	

	// Check every object in the path of the focus.
	for (int index{ 0 }; index < collisionListCurrentIndex_; ++index)
	{		
		const Tile* tile{ collisionList_[index]->obstacleTile_ };

		// Reset these each loop. They will be used to adjust focus velocity if collision with an obstacle happens.		
		Vector2 contactNormal{ 0.0f, 0.0f };
		float timeToContact{ 8.0f };

		// Get the target hitbox (hitbox on the tile) and position.
		const Hitbox& targetHitbox{ *(tile->hitboxes_[0]) };
		const int2& targetPosition{ collisionList_[index]->position };

		/* 1ST DEGREE COLLISION CHECK - Does the collider around each object touch ? */
		if (raycaster_->FocusVsHitbox(focus, targetHitbox, targetPosition, timeToContact)
			&& timeToContact < 1.0f)
		{
			// If successful, then check each hitbox of the focus against the obstacle's hitbox.
			for (int hitboxIndex{ 0 }; hitboxIndex < focus->hitboxCount_; ++hitboxIndex)
			{
				Hitbox& focusHitbox{ focus->hitboxes_[hitboxIndex] };

				// Only compare hitboxes if the focus hitbox has an obstacle mode and the focus mask cares about the targt.
				if ((focusHitbox.mode_ & tileModes_) && (focusHitbox.tileMask_ & targetHitbox.tileLayer_))
				{
					/* 2ND DEGREE COLLISION CHECK - Do any of the hitboxes of the focus actually touch the collider of the target ? */
					if (raycaster_->HitboxVsHitbox(focus, focusHitbox, targetHitbox, targetPosition, timeToContact, contactNormal) 
						&& timeToContact < 1.0f)
					{						
						AdjustForObstacleCollision(focus, contactNormals, contactNormal, timeToContact);						
					}
				}
			}
		}
	}
	
	NotifyFocusOfObstacleCollision(focus, contactNormals);
}


// Focus velocity /must/ be adjusted when a collision is found before checking the next collision item.
void ObstacleCollisionManager::AdjustForObstacleCollision(GameObject* const focus, Vector2& contactNormals, const Vector2& contactNormal, const float timeToContact)
{
	contactNormals += contactNormal;
	*(focus->frameVelocity_) += contactNormal * focus->frameVelocity_->Absolute() * (1.0f - timeToContact);
}


void ObstacleCollisionManager::NotifyFocusOfObstacleCollision(GameObject* const focus, const Vector2& contactNormals)
{
	focus->ResolveObstacleCollisions(static_cast<float2>(contactNormals));
}


/*
* UNUSED DDA PIXEL CHECK CODE. TRY AGAIN WHEN ENTIRE PROCESS IS DDA INSTEAD OF RAYCAST.
// [Credit] https://www.youtube.com/watch?v=NbSee-XM7WA
const bool CM::CheckForPixelCollision(GameObject* const focus, Hitbox& focusHitbox, const Tile& targetTile, const int2& targetPosition,
	Vector2& entryPoint, Vector2& endPoint, int2& location)
{
	// Apply floating-point origin to entry and end points.
	entryPoint += focus->shape_.start_ + focusHitbox.GetCenter();
	endPoint += focus->shape_.start_ + focusHitbox.GetCenter();

	// Corner to advance through DDA.
	float2 hitboxStart{ entryPoint.x - (focusHitbox.size_.x * 0.5f), entryPoint.y - (focusHitbox.size_.y * 0.5f) };

	// Distance to advance through DDA.
	float x{ (entryPoint.x - endPoint.x) };
	float y{ (entryPoint.y - endPoint.y) };
	float lengthToCheck{ sqrtf(powf((entryPoint.x - endPoint.x), 2.0) + powf((entryPoint.y - endPoint.y), 2.0)) };

	// Direction of the ray.
	Vector2 rayDirection{ normalize(static_cast<float2>(focus->velocity_)) };

	// Scaling factor to find length of hypotenuse when 1 x or y unit is added.
	Vector2 rayUnitStepSize{
		sqrt(1 + (rayDirection.y / rayDirection.x) * (rayDirection.y / rayDirection.x)),
		sqrt(1 + (rayDirection.x / rayDirection.y) * (rayDirection.x / rayDirection.y))
	};

	// Other helper variables.
	location = static_cast<int>(hitboxStart.x), static_cast<int>(hitboxStart.y);
	float2 rayLength{ 0.0f, 0.0f };
	int2 step{ 0, 0 };

	// Find initial distances to edge of pixel (since we rarely will start at the corner of a pixel).
	if (rayDirection.x < 0.0f)
	{
		step.x = -1;
		rayLength.x = (hitboxStart.x - location.x) * rayUnitStepSize.x;
	}
	else
	{
		step.x = 1;
		rayLength.x = (location.x + 1.0f - hitboxStart.x) * rayUnitStepSize.x;
	}

	if (rayDirection.y < 0.0f)
	{
		step.y = -1;
		rayLength.y = (hitboxStart.y - location.y) * rayUnitStepSize.y;
	}
	else
	{
		step.y = 1;
		rayLength.y = (location.y + 1.0f - hitboxStart.y) * rayUnitStepSize.y;
	}

	// Walk along grid (screen pixels) in direction of ray.
	// Do pixel collision after each step until either collision is found or we reach the exit point.
	float checkedProgress{ 0.0f };
	int2 lastStep{ 0, 0 };
	bool skipX{ rayUnitStepSize.x == 0.0f || isinf(rayUnitStepSize.x) };
	bool skipY{ rayUnitStepSize.y == 0.0f || isinf(rayUnitStepSize.y) };

	// If somehow they both are skipped, just stop the player at entry point.
	if (skipX && skipY)
	{
		return true;
	}

	do
	{
		// Pixel check. Apply location values to focus to check new overlaps as focus 'marches' through the object.
		if (true)//PixelCheck(focus, focusHitbox, targetTile, targetPosition, location))
		{
			location.x -= lastStep.x;
			location.y -= lastStep.y;

			// Collision.
			return true;
		}

		// Walk to next pixel.
		if (!skipX && (rayLength.x < rayLength.y || skipY))
		{
			location.x += step.x;
			lastStep = { step.x, 0 };
			checkedProgress = rayLength.x;
			rayLength.x += rayUnitStepSize.x;
		}
		else if (!skipY && (rayLength.y < rayLength.x || skipX))
		{
			location.y += step.y;
			lastStep = { 0, step.y };
			checkedProgress = rayLength.y;
			rayLength.y += rayUnitStepSize.y;
		}

	} while (checkedProgress < lengthToCheck);

	// No collision.
	return false;
}


void CM::RecalculateTimeToContact(GameObject* const focus, Hitbox& focusHitbox, const int2& finalLocation, float& timeToContact)
{
	float temp{ timeToContact };

	Vector2 origin{ trunc(focus->shape_.start_.x) + focusHitbox.start_.x, trunc(focus->shape_.start_.y) + focusHitbox.start_.y };
	Vector2 end{ finalLocation };

	Vector2 tFinal{ (end - origin) / focus->velocity_ };

	if (isnan(tFinal.x))
	{
		timeToContact = tFinal.y;
	}
	else if (isnan(tFinal.y))
	{
		timeToContact = tFinal.x;
	}
	else
	{
		timeToContact = max(tFinal.x, tFinal.y);
	}
}
*/