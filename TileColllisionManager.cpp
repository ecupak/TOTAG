#include "precomp.h"
#include "TileColllisionManager.h"

#include "CollisionFinder.h"
#include "Raycaster.h"
#include "GameObject.h"
#include "ObjectPooler.h"
#include "Hitbox.h"
#include "Shape.h"

#include "Level.h"
#include "MapData.h"
#include "TilesetData.h"
#include "PixelChecker.h"

#include "TileCollision.h"


// Used to set array sizes without a magic number.
constexpr int collisionListMaxSize_{ 50 };


TileColllisionManager::TileColllisionManager(const Level& level, const MapData& mapData, const ObjectPooler& objectPooler)
	: level_{ level }
	, mapData_{ mapData }
	, collisionList_{ new const MapTile*[collisionListMaxSize_] }
	, objectPooler_{ objectPooler }
	, collisionFinder_{ new CollisionFinder{ level_, mapData_ } }
	, raycaster_{ new Raycaster{} }
	, pixelChecker_{ new PixelChecker{} }
{	}


TileColllisionManager::~TileColllisionManager()
{
	delete[] collisionList_;
	delete collisionFinder_;
	delete raycaster_;
}


void TileColllisionManager::ProcessCollisions(const float deltaTime)
{
	for (int index{ 0 }; index < 1 /*objectPooler_.GetSize()*/ ; ++index)
	{
		GameObject* const focus{ objectPooler_[index] };

 		collisionFinder_->FindCollisionsAgainstFocus(focus, deltaTime, collisionList_, collisionListCurrentIndex_);

		//FindEndOfCollisionList();

		SortObjectsByDistance(focus);

		CheckObjectsAgainstFocus(focus, deltaTime);
	}
}


// Find size of collisionList.
void TileColllisionManager::FindEndOfCollisionList()
{
	for (int index{ 0 }; index < collisionListMaxSize_; ++index)
	{
		if (collisionList_[index] == nullptr)
		{
			collisionListCurrentIndex_ = index;
			break;
		}
	}
}


void TileColllisionManager::SortObjectsByDistance(GameObject* const focus)
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

	// Calculate and store distances from objects to focus.
	for (int index{ 0 }; index < collisionListCurrentIndex_; ++index)
	{
		float xDiff = collisionList_[index]->position.x - focus->shape_->start_.x;
		float yDiff = collisionList_[index]->position.y - focus->shape_->start_.y;

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


// Cascading collision checks. If a collision is possible, narrow it down to the pixel level to confirm.
void TileColllisionManager::CheckObjectsAgainstFocus(GameObject* const focus, const float deltaTime)
{
	// Data to be returned to the focus.
	Vector2 contactNormals{ 0.0f, 0.0f };
	
	// Passed to the focus after collision is resolved.
	Vector2 entryPoint{ 0.0f, 0.0f };

	// For ever object in the path of the focus, check...
	for (int index{ 0 }; index < collisionListCurrentIndex_; ++index)
	{
		// Extract the 2 possible tiles that can be at this map location.
		//const Tile* mapTile[2]{ collisionList_[index]->obstacleTile_, collisionList_[index]->interactTile_ };
		const Tile* tile{ collisionList_[index]->obstacleTile_ };

		// Loop for each tile...
		//for (int tileIndex{ 0 }; tileIndex < 2; ++tileIndex)
		{
			/*
			// If the tile isn't assigned, skip it. (At least 1 tile will be assigned. Otherwise the mapTile's collisionLayer would be 0 and of no interest to the focus.)
			if (mapTile[tileIndex] == nullptr)
			{
				continue;
			}

			// If the focus isn't interested in the tile, skip it. (At least 1 tile will be of interest. Otherwise the mapTile would not have been added to the collision list.)
			if (!(focus->collisionMask_ & mapTile[tileIndex]->hitboxes_[0]->collisionLayer_))
			{
				continue;
			}
			*/

			// Reset these each loop. They will be used to adjust focus velocity if collision with an obstacle happens.
			entryPoint = float2{ 0.0f, 0.0f };			
			Vector2 endPoint{ 0.0f, 0.0f };
			Vector2 contactNormal{ 0.0f, 0.0f };
			float timeToContact{ 8.0f };			

			// Get the target hitbox (hitbox on the tile) and position.
			const Hitbox& targetHitbox{ *(tile->hitboxes_[0]) };
			const int2& targetPosition{ collisionList_[index]->position };

			// 1ST DEGREE COLLISION CHECK - Does the collider around each object touch? //
			
			// Bounding box check. // Replace with circle distance check? // AABB vs AABB?
			if (raycaster_->FocusVsHitbox(focus, targetHitbox, targetPosition, timeToContact)
				&& timeToContact < 1.0f)
			{
				// Check each hitbox of the focus.
				for (int hitboxIndex{ 0 }; hitboxIndex < focus->hitboxCount_; ++hitboxIndex)
				{
					Hitbox& focusHitbox{ focus->hitboxes_[hitboxIndex] };

					// Only compare hitboxes if the focus hitbox is not a trigger and the focus mask cares about the targt.
					if (focusHitbox.isParticipating_ && (focusHitbox.tileMask_ & targetHitbox.tileLayer_))
					{
						// 2ND DEGREE COLLISION CHECK - Do any of the hitboxes of the focus actually touch the collider of the target? //
						
						// Confirm if the hitboxes collide. Store additional data for future processing.
						if (raycaster_->HitboxVsHitbox(focus, focusHitbox, targetHitbox, targetPosition,
							timeToContact, entryPoint, endPoint, contactNormal) && timeToContact < 1.0f)
						{
							// 3RD DEGREE COLLISION CHECK - Does the sprite of the focus touch the sprite of the target? //
							
							// SKIPPING STEP. CANNOT MAKE WORK RELIABLY WITH THE RAYCAST COLLISION METHOD. WILL TRY LATER WITH DDA FOR ALL COLLISION //							
							// Pixel vs pixel check. Starting at collision point, advance entire focus 1 pixel at a time along direction/velocity. Scan overlapping pixels for hits.
							//int2 location{ 0, 0 };
							// if (CheckForPixelCollision(focus, focusHitbox, *(mapTile[tileIndex]), targetPosition, entryPoint, endPoint, location))
							{
								AdjustForObstacleCollision(focus, contactNormals, contactNormal, timeToContact);
							}
						}
					}
				}
			}
		}
	}
	
	NotifyFocusOfObstacleCollision(focus, contactNormals, deltaTime);
	SetHitboxOverlaps(focus);
}




// Focus velocity /must/ be adjusted when a collision is found before checking the next collision item.
void TileColllisionManager::AdjustForObstacleCollision(GameObject* const focus, Vector2& contactNormals, const Vector2& contactNormal, const float timeToContact)
{
	contactNormals += contactNormal;
	*(focus->frameVelocity_) += contactNormal * focus->frameVelocity_->Absolute() * (1.0f - timeToContact);
}


// This is a discrete check at the end of movement to see what the focus is overlapping.
void TileColllisionManager::SetHitboxOverlaps(GameObject* const focus)
{
	// debug. clears the shadow sprite.
	focus->hitboxes_[3].source_->GetSurface()->Clear(0);

	// A list of all overlapped tiles regardless of layers/masks.
	focus->overlappingTileLayers_ = 0;

	// For each hitbox the focus has, find the interact tiles it touches. Also add the obstacle collision layers together for the focus to use.
	for (int hitboxIndex{ 0 }; hitboxIndex < focus->hitboxCount_; ++hitboxIndex)
	{
		Hitbox& hitbox{ focus->hitboxes_[hitboxIndex] };

		// Erase old data in the hitbox.
		for (int index{ 0 }; index < 4; ++index)
		{
			hitbox.overlappingTiles_[index] = nullptr;
			hitbox.contactPoints_[index] = { -1, -1 };
			hitbox.contactHitboxes_[index] = -1;
		}

		// Setup loop variables.
		int2 hitboxStart{
			max(0, static_cast<int>(focus->shape_->start_.x) + hitbox.start_.x),
			max(0, static_cast<int>(focus->shape_->start_.y) + hitbox.start_.y)
		};

		int2 hitboxEnd{
			min(level_.GetTileSize().x * TILE_WIDTH,hitboxStart.x + hitbox.size_.x),
			min(level_.GetTileSize().y * TILE_HEIGHT,hitboxStart.y + hitbox.size_.y)
		};

		// Get the top-left tile being overlapped. Find its world position origin. Increment by tile size until the value is beyond the hitbox's x/y end value.
		int2 initialTilePosition{ (hitboxStart.x / TILE_WIDTH) * TILE_WIDTH, (hitboxStart.y / TILE_HEIGHT) * TILE_HEIGHT };

		// At most, a hitbox can only touch 4 other tiles (since no object or hitbox is larger than a single tile).
		int index{ 0 };
		int2 tilePos{ 0,0 };
		for (tilePos.y = initialTilePosition.y; tilePos.y < hitboxEnd.y; tilePos.y += TILE_HEIGHT)
		{
			for (tilePos.x = initialTilePosition.x; tilePos.x < hitboxEnd.x; tilePos.x += TILE_WIDTH)
			{		
				// Get the mapTile under the tilePos.
				const MapTile& mapTile{ mapData_.GetTileAtPosition(tilePos.x, tilePos.y) };
				
				// Add mapTile to hitbox list if the hitbox mask cares about the tiles layer.
				if (mapTile.interactTile_ != nullptr && (hitbox.tileMask_ & mapTile.interactTile_->hitboxes_[0]->tileLayer_))
				{
					for (int tileHitboxIndex{ 0 }; tileHitboxIndex < mapTile[TileType::Interact]->hitboxCount_; ++tileHitboxIndex)
					{
						const Hitbox& tileHitbox{ *(mapTile[TileType::Interact]->hitboxes_[tileHitboxIndex]) };

						int2 tileStart{ tilePos.x + tileHitbox.start_.x, tilePos.y + tileHitbox.start_.y };
						int2 tileEnd{ tileStart.x + tileHitbox.size_.x, tileStart.y + tileHitbox.size_.y };

						// AABB check to make sure interact object and focus hitbox are touching.
						if ((hitboxEnd.x > tileStart.x && tileEnd.x > hitboxStart.x) && (hitboxEnd.y > tileStart.y && tileEnd.y > hitboxStart.y))
						{
							// Pixel check to ensure the sprites touch.
							int2 contactPoint{ 0, 0 };
							if (hitbox.hasNoPixels_ || pixelChecker_->DoesFocusTouchTile(*focus, hitbox, contactPoint, tileHitbox, mapTile, TileType::Interact))
							{
								hitbox.overlappingTiles_[index] = &mapTile;
								hitbox.contactPoints_[index] = contactPoint;
								hitbox.contactHitboxes_[index] = tileHitboxIndex;
								++index;
								
								// After first collision with a hitbox in a tile, no need to check other hitboxes in same tile.
								break;
							}
						}
					}
				}

				// Add obstacle and interact collision layer to focus's overlapping obstacle list regardless of interest.
				if (hitbox.isParticipating_)
				{
					int obstacleCollisionLayer{ mapTile[TileType::Obstacle] == nullptr ? 0 : mapTile[TileType::Obstacle]->hitboxes_[0]->tileLayer_ };
					int interactCollisionLayer{ mapTile[TileType::Interact] == nullptr ? 0 : mapTile[TileType::Interact]->hitboxes_[0]->tileLayer_ };
					focus->overlappingTileLayers_ |= (obstacleCollisionLayer | interactCollisionLayer);
				}
			}
		}
	}
}


void TileColllisionManager::NotifyFocusOfObstacleCollision(GameObject* const focus, const Vector2& contactNormals, const float deltaTime)
{
	focus->ResolveTileCollisions(static_cast<float2>(contactNormals), deltaTime);

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