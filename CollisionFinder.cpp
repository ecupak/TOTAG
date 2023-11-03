#include "precomp.h"
#include "CollisionFinder.h"

#include "GameObject.h"
#include "Shape.h"
#include "Hitbox.h"

#include "MapData.h"
#include "TilesetData.h"


CollisionFinder::CollisionFinder(const int2& tileSize, const MapData& mapData)
	: tileSize_{ tileSize }
	, mapData_{ mapData }
{	}


void CollisionFinder::FindCollisionsAgainstFocus(GameObject const* focus, const MapTile* collisionList[], int& collisionListSize) const
{
	Shape boundingBox;

	AdjustBoundingBoxToFocus(focus, boundingBox);

	FillCollisionList(focus, boundingBox, collisionList, collisionListSize);
}


// Create bounding box that surrounds the focus at its current and future positions.
void CollisionFinder::AdjustBoundingBoxToFocus(GameObject const* focus, Shape& boundingBox) const
{
	// Find corners of bounding box by using focus and the destination of focus.
	float2 boundingBoxStart{
		min(trunc(focus->shape_->start_.x), trunc(focus->shape_->start_.x) + focus->frameVelocity_->x),
		min(trunc(focus->shape_->start_.y), trunc(focus->shape_->start_.y) + focus->frameVelocity_->y)
	};

	float2 boundingBoxEnd{
		max(focus->shape_->end_.x, focus->shape_->end_.x + focus->frameVelocity_->x),
		max(focus->shape_->end_.y, focus->shape_->end_.y + focus->frameVelocity_->y)
	};

	// Adjust bounding box.
	boundingBox.size_ = float2{
		boundingBoxEnd.x - boundingBoxStart.x + 1,
		boundingBoxEnd.y - boundingBoxStart.y + 1,
	};

	boundingBox.SetPosition(boundingBoxStart);
}


// For each tile length along the bounding box, sample the tile at world position and add it if it matches the collision mask of focus.
void CollisionFinder::FillCollisionList(GameObject const* focus, Shape& boundingBox, const MapTile* collisionList[], int& collisionListCurrentIndex) const
{
	// World position of bounding box corners constrained to be within the map.
	int2 start{ 
		max(0, static_cast<int>(boundingBox.start_.x)),
		max(0, static_cast<int>(boundingBox.start_.y))
	};

	int2 end{ 
		min(tileSize_.x * TILE_WIDTH, static_cast<int>(boundingBox.end_.x) + 1),
		min(tileSize_.y * TILE_HEIGHT, static_cast<int>(boundingBox.end_.y) + 1)
	};

	collisionListCurrentIndex = 0;

	// Get the top-left tile being overlapped. Find it's world position origin. Now in all subsequent loops, advance by tilesize until beyond the camera's x/y end.
	int2 initialTilePosition{ (start.x / TILE_WIDTH) * TILE_WIDTH, (start.y / TILE_HEIGHT) * TILE_HEIGHT };

	for (int y{ initialTilePosition.y }; y < end.y; y += TILE_HEIGHT)
	{
		for (int x{ initialTilePosition.x }; x < end.x; x += TILE_WIDTH )
		{
			const MapTile& mapTile{ mapData_.GetTileAtPosition(x, y) };

			// If obstacle's collision layer matches the collision mask of focus, add to list.
			if (mapTile.obstacleTile_ != nullptr && (mapTile.obstacleTile_->hitboxes_[0]->tileLayer_ & focus->tileMask_))
			{
				collisionList[collisionListCurrentIndex] = &mapTile;
				++collisionListCurrentIndex;
			}
		}
	}
}