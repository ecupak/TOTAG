#include "precomp.h"
#include "HotspotCollisionManager.h"

#include "Level.h"
#include "Raycaster.h"
#include "TileCollisionEnum.h"

#include <iostream>

/*
struct CollisionResult
{
public:
	CollisionResult()
		: contactNormals{ 0.0f, 0.0f }
		, environmentalObject{ 0, 0 }
		, entryPoint{ 0.0f, 0.0f }
		, endPoint{0.0f, 0.0f }
	{	}


	void Reset()
	{
		contactNormals = Vector2{ 0.0f, 0.0f };
		entryPoint = Vector2{ 0.0f, 0.0f };
		endPoint = Vector2{ 0.0f, 0.0f };
	}

	Vector2 contactNormals{ 0.0f, 0.0f };
	GameObject environmentalObject{ 0, 0 };
	Vector2 entryPoint{ 0.0f, 0.0f };
	Vector2 endPoint{ 0.0f, 0.f };
};


HotspotCollisionManager::HotspotCollisionManager(ObjectPooler& tileObjectPool, Level& level)
	: tileObjectPool_{ tileObjectPool }
	, level_{ level }
{	
	raycaster_ = new Raycaster{};
}


void HotspotCollisionManager::ProcessCollisions()
{
	CollisionResult collisionResult{};

	for (GameObject* focus : tileObjectPool_.GetPool())
	{
		collisionResult.Reset();

		GetCollisionsFor(focus, collisionResult);
		NotifyCollisionsFor(focus, collisionResult);
		NotifyCornersFor(focus);
	}
}


void HotspotCollisionManager::GetCollisionsFor(GameObject* focus, CollisionResult& collisionResult)
{
	// REPLACE THIS SECTION WITH DAA SETUP? //
	
	// Create bounding box around focus and destination.
	Shape boundingBox;
	CreateBoundingBox(focus, boundingBox);


	// Get object list of possible collisions.
	Vector<GameObject> objectList;
	objectList.Reserve(8);
	FindTileCollisions(focus, boundingBox, objectList);

	// END DDA SECTION //

	
	// SORT OBJECTS BY DISTANCE TO RAYCAST ORIGIN //
	
	results_.Reserve(objectList.Size());

	// Get distance from raycast source.
	Vector2 playerCenter{ focus->shape_.center_ };

	for (int index{ 0 }; index < objectList.Size(); ++index)
	{
		float xDiff = objectList[index].shape_.center_.x - playerCenter.x;
		float yDiff = objectList[index].shape_.center_.y - playerCenter.y;

		results_.Push({ &objectList[index], (xDiff * xDiff) + (yDiff * yDiff) });
	}


	// Bubble sort based on distance to raycast source. Closest obstacles should be resolved first.
	SortData temp{ nullptr, 0.0f };
	for (int position{ 0 }; position < objectList.Size() - 1; ++position)
	{
		for (int compareTo{ position + 1 }; compareTo < objectList.Size(); ++compareTo)
		{
			if (results_[position].distanceAway > results_[compareTo].distanceAway)
			{
				temp = results_[position];
				results_[position] = results_[compareTo];
				results_[compareTo] = temp;
			}
		}
	}

	// END SORT //


	// Find collisions in order of closest to furthest objects.
	Vector2 contactNormal{ 0.0f, 0.0f };
	Vector2 entryPoint{ 0.0f, 0.0f };
	Vector2 endPoint{ 0.0f, 0.0f };
	
	float timeToContact{ 0.0f };
	int environmentalTileIds{ TileCollisionID::Spike | TileCollisionID::Trampoline | TileCollisionID::Zipline };
	bool hasHitEnvironmentalTile{ false };

	for (int index{ 0 }; index < results_.Size(); ++index)
	{
		GameObject& object{ *results_[index].object };

		// If raycast detects a hit on the object ...
		if (raycaster_->DynamicRaycastToObject(focus, object, entryPoint, endPoint, contactNormal, timeToContact)
			&& timeToContact < 1.0f)
		{
			// ... And it is the first environmental tile hit:
			if (object.collisionLayer_ & environmentalTileIds)
			{
				if (!hasHitEnvironmentalTile)
				{
					collisionResult.environmentalObject = object;
					collisionResult.entryPoint = entryPoint;
					collisionResult.endPoint = endPoint;
					hasHitEnvironmentalTile = true;
				}
			}
			// ... Otherwise, if the tile is not empty, we collided with it:
			else if (object.collisionLayer_ & ~(TileCollisionID::Nothing))
			{
				collisionResult.contactNormals += contactNormal;
				focus->velocity_ += contactNormal * focus->velocity_.Absolute() * (1.0f - timeToContact);
			}
		}
	}
}


void HotspotCollisionManager::CreateBoundingBox(GameObject* focus, Shape& boundingBox)
{
	Shape destinationBox;
	
	// Move to expected position after velocity is applied.
	destinationBox.size_ = float2{TILE_WIDTH, TILE_HEIGHT};
	destinationBox.SetCenter(focus->shape_.center_ + focus->velocity_);

	float2 start{
		min(destinationBox.start_.x, focus->shape_.start_.x),
		min(destinationBox.start_.y, focus->shape_.start_.y)
	};

	float2 end{
		max(destinationBox.end_.x, focus->shape_.end_.x),
		max(destinationBox.end_.y, focus->shape_.end_.y)
	};


	// Apply resizing to bounding box (will enclose player and intended collision box).
	boundingBox.size_ = float2{
		end.x - start.x + 1,
		end.y - start.y + 1
	};
	boundingBox.SetPosition(start);
}


void HotspotCollisionManager::FindTileCollisions(GameObject* focus, Shape& boundingBox, Vector<GameObject>& objects)
{
	const int2 start{ 
		static_cast<int>(boundingBox.start_.x), 
		static_cast<int>(boundingBox.start_.y) 
	};

	const int2 end{ 
		static_cast<int>(boundingBox.end_.x), 
		static_cast<int>(boundingBox.end_.y) 
	};

	// Start with topleft of collision box and check every point at distances equal to the tile width and height.
	// Also explicitly check points along right and bottom edge as they will usually not be exactly divisible by tile width and height.
	GameObject go{0, 0};

	int yIndex{ -1 };
	int xIndex{ -1 };

	for (int y = start.y; y <= end.y;)
	{
		yIndex = GetTileRowAtPosition(y);

		for (int x = start.x; x <= end.x;)
		{
			xIndex = GetTileColumnAtPosition(x);

			// Collision id of tile must match the collision mask from our current state to count as a collision.
			int collisionId{ GetTileCollisionIdAtIndex(xIndex, yIndex) };

			if (collisionId & focus->collisionMask_)
			{
				// Assign info.
				go.collisionLayer_ = collisionId;
				go.shape_.size_ = float2{ TILE_WIDTH, TILE_HEIGHT };
				go.shape_.SetPosition(Vector2{ floorf(1.0f * x / TILE_WIDTH) * TILE_WIDTH, floorf(1.0f * y / TILE_HEIGHT) * TILE_HEIGHT }); // topleft corner of tile.
			
				objects.Push(go);
			}

			// We must hit the last value before the loop ends - but only if the final position gives a new index.
			if (x != end.x && x + TILE_WIDTH > end.x)
			{
				int nextXIndex{ GetTileColumnAtPosition(end.x) };
				if (xIndex != nextXIndex)
				{
					x = end.x;
				}
				else
				{
					break;
				}
			}
			else
			{
				x += TILE_WIDTH;
			}
		}

		// We must hit the last value before the loop ends - but only if the final position gives a new index. 
		if (y != end.y && y + TILE_HEIGHT > end.y)
		{
			int nextYIndex{ GetTileRowAtPosition(end.y) };
			if (yIndex != nextYIndex)
			{
				y = end.y;
			}
			else
			{
				break;
			}
		}
		else
		{
			y += TILE_HEIGHT;
		}
	}
}


void HotspotCollisionManager::NotifyCollisionsFor(GameObject* focus, CollisionResult& collisionResult)
{
	focus->OnHotspotCollision(
		collisionResult.environmentalObject,
		collisionResult.entryPoint,
		collisionResult.endPoint,
		collisionResult.contactNormals
	);
}


void HotspotCollisionManager::NotifyCornersFor(GameObject* focus)
{
	// Check 4 corners of focus. If it touches a rope tile, compare the rope tile to the focus sprite to see if there is a pixel overlap.
	// Set that corner to the tile collision id of Nothihng or Rope.

	// Get focus sprite.
	const unsigned int* focusPixel{ focus->sprite_->GetBuffer() };

	int2 focusStart{
		static_cast<int>(focus->shape_.start_.x),
		static_cast<int>(focus->shape_.start_.y)
	};

	int2 focusEnd{
		static_cast<int>(focus->shape_.end_.x),
		static_cast<int>(focus->shape_.end_.y)
	};

	// For each corner of the hitbox, find the underlying tile.
	// If the underlying tile is the same as a previous corner's result, skip it.
	// If there is a pixel collision, add that collision layer id to hitbox.
	// Otherwise, set as nothing.

	int tilemapWidth{ level_.ruinsDayTilemapSprite_->GetSurface()->width };


	// Check for ropes, obstacles, and ziplines.
	Level::TileLayer layers[3] = {
		Level::TileLayer::Obstacle,
		Level::TileLayer::Rope,
		Level::TileLayer::Zipline
	};


	TileCollisionID ids[3] = {
		TileCollisionID::Obstacle,
		TileCollisionID::Rope,
		TileCollisionID::Zipline
	};

	// Reset corners.
	for (int cornerIndex{ 0 }; cornerIndex < 4; ++cornerIndex)
	{
		//focus->cornerIds_[cornerIndex] = TileCollisionID::Nothing;
	}

	bool printTile = false;

	// Set new values.
	for (int layerIndex{ 0 }; layerIndex < 3; ++layerIndex)
	{
		for (int cornerIndex{ 0 }; cornerIndex < 4; ++cornerIndex)
		{
			bool skipToNext{ false };

			int2 cornerPos{ GetCornerPosition(cornerIndex, focus) };

			// Get sprite tile under corner.
			int tileId{ level_.GetTileAt(layers[layerIndex], cornerPos.x, cornerPos.y) };

			// If no tile, early answer.
			if (tileId == 0)
			{
				continue;
			}

			// This is the tile's topleft corner on the level map/world.
			int2 tilePosition{
				static_cast<int>(floor(cornerPos.x / TILE_WIDTH) * TILE_WIDTH),
				static_cast<int>(floor(cornerPos.y / TILE_HEIGHT) * TILE_HEIGHT)
			};

			// Find overlap rectangle of tile and full focus.
			int2 start{
				max(focusStart.x, tilePosition.x),
				max(focusStart.y, tilePosition.y)
			};

			int2 end{
				min(focusStart.x + TILE_WIDTH, tilePosition.x + TILE_WIDTH),
				min(focusStart.y + TILE_HEIGHT, tilePosition.y + TILE_HEIGHT)
			};

			// Loop over the overlap area and if the tile has a non-zero pixel value, check against the player sprite.
			const unsigned int* tilePixel{ level_.GetSpriteWithTileId(tileId) };
			int tileOffset{ (start.x - tilePosition.x) + ((start.y - tilePosition.y) * tilemapWidth) };
			int focusOffset{ (start.x - focusStart.x) + ((start.y - focusStart.y) * TILE_WIDTH) };

			if (printTile)
			{
				PrintTile(tilePixel, tileOffset);
			}

			for (int y{ start.y }; y < end.y; ++y)
			{
				for (int x{ 0 }; x < (end.x - start.x); ++x)
				{
					// If the tile has a non-zero pixel at this position.
					int value = tilePixel[tileOffset + x];
					if (tilePixel[tileOffset + x] & 0xFFFFFF)
					{
						// And if the focus also has a non-zero pixel at the same location.
						if (focusPixel[focusOffset + x] & 0xFFFFFF)
						{
							// We have a collision.
							//focus->cornerIds_[cornerIndex] |= ids[layerIndex];
							skipToNext = true;
							break;
						}
					}
				}

				if (skipToNext)
				{
					break;
				}

				tileOffset += tilemapWidth;
				focusOffset += TILE_HEIGHT;
			}
		}

	}
}
 

void HotspotCollisionManager::PrintTile(const unsigned int* pixelStart, int tileOffset)
{
	int offset = tileOffset;

	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			std::cout << (pixelStart[offset + x] & 0xFFFFFF ? "1 " : "0 ");
		}

		std::cout << '\n';
		offset += level_.ruinsDayTilemapSprite_->GetSurface()->width;
		std::cout << offset << '\n';
	}

	std::cout << tileOffset << '\n';
}

int2 HotspotCollisionManager::GetCornerPosition(int cornerIndex, GameObject* focus)
{
	float x{ 0 };
	float y{ 0 };

	switch (cornerIndex)
	{
	case 0:
		x = focus->shape_.start_.x;
		y = focus->shape_.start_.y;
		break;
	case 1:
		x = focus->shape_.end_.x;
		y = focus->shape_.start_.y;
		break;
	case 2:
		x = focus->shape_.start_.x;
		y = focus->shape_.end_.y;
		break;
	case 3:
		x = focus->shape_.end_.x;
		y = focus->shape_.end_.y;
		break;
	default:
		break;
	}

	return { static_cast<int>(x), static_cast<int>(y) };
}


const int& HotspotCollisionManager::GetCollisionLayerOfTileAtPosition(const int2& position) const
{
	return GetCollisionLayerOfTileAtPosition(position.x, position.y);
}


const int& HotspotCollisionManager::GetCollisionLayerOfTileAtPosition(const int x, const int y) const
{
	return level_.GetCollisionIds()[static_cast<int>(trunc(x / TILE_WIDTH) + (trunc(y / TILE_HEIGHT) * level_.GetSize().x))];
}


const int& HotspotCollisionManager::GetTileCollisionIdAtIndex(const int x, const int y) const
{
	return level_.GetCollisionIds()[x + (y * level_.GetSize().x)];
}


const int HotspotCollisionManager::GetTileRowAtPosition(const int y) const
{
	return static_cast<int>(trunc(1.0f * y / TILE_HEIGHT));
}


const int HotspotCollisionManager::GetTileColumnAtPosition(const int x) const
{
	return static_cast<int>(trunc(1.0f * x / TILE_WIDTH));
}


const bool HotspotCollisionManager::IsObjectTouchingCollisionId(GameObject* object, const int collisionId, const float2& offset)
{
	Shape& shape{ object->shape_ };

	return(
		GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.start_.x + offset.x)), static_cast<int>(trunc(shape.start_.y + offset.y))) & collisionId
		|| GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.end_.x + offset.x)), static_cast<int>(trunc(shape.start_.y + offset.y))) & collisionId
		|| GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.start_.x + offset.x)), static_cast<int>(trunc(shape.end_.y + offset.y))) & collisionId
		|| GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.end_.x + offset.x)), static_cast<int>(trunc(shape.end_.y + offset.y))) & collisionId
		);
}


const bool HotspotCollisionManager::IsCornerTouchingCollisionId(GameObject* object, const int collisionId, float2& corner, const float2& offset)
{
	// Find corner that first touched.
	int cornerId{ 0 };

	Shape& shape{ object->shape_ };

	if (
		GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.start_.x + offset.x)), static_cast<int>(trunc(shape.start_.y + offset.y))) & collisionId
		|| (++cornerId && GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.end_.x + offset.x)), static_cast<int>(trunc(shape.start_.y + offset.y))) & collisionId)
		|| (++cornerId && GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.start_.x + offset.x)), static_cast<int>(trunc(shape.end_.y + offset.y))) & collisionId)
		|| (++cornerId && GetCollisionLayerOfTileAtPosition(static_cast<int>(trunc(shape.end_.x + offset.x)), static_cast<int>(trunc(shape.end_.y + offset.y))) & collisionId)
		)
	{
		switch (cornerId)
		{
		case 0:
			corner = { shape.start_.x, shape.start_.y };
			break;
		case 1:
			corner = { shape.end_.x, shape.start_.y };
			break;
		case 2:
			corner = { shape.start_.x, shape.end_.y };
			break;
		case 3:
			corner = { shape.end_.x, shape.end_.y };
			break;
		}

		return true;
	}
	else
	{
		return false;
	}
}
*/