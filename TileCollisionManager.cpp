#include "precomp.h"
#include "TileCollisionManager.h"

#include "GameObject.h"
#include "Shape.h"
#include "Hitbox.h"
#include "Level.h"
#include "MapData.h"
#include "TilesetData.h"
#include "PixelChecker.h"


TileCollisionManager::TileCollisionManager(const int2& tileSize, const MapData& mapData, GameObject* const activeList[])
	: tileSize_{ tileSize }
	, mapData_{ mapData }
	, tileModes_{ Hitbox::Mode::Map | Hitbox::Mode::Interact | Hitbox::Mode::InteractPlus }
	, activeList_{ activeList }
	, pixelChecker_{ new PixelChecker{} }
{	}


TileCollisionManager::~TileCollisionManager()
{
	delete pixelChecker_;
}


void TileCollisionManager::ProcessCollisions(const int activeCount)
{
	for (int index{ 0 }; index < activeCount; ++index)
	{
		GameObject* const focus{ activeList_[index] };

		// Only actors need to know what tiles they overlap.
		// Props are objects that interact only with actors and/or obstacles.
		if (focus->objectType_ == ObjectType::Actor && focus->isOnstage_)
		{
			SetHitboxOverlaps(focus);
		}
	}
}


// This is a discrete check at the end of movement to see what the focus is overlapping.
void TileCollisionManager::SetHitboxOverlaps(GameObject* const focus)
{
	// A list of all overlapped tiles regardless of layers/masks.
	focus->overlappingTileLayers_ = 0;

	// Check each hitbox of the focus.
	for (int hitboxIndex{ 0 }; hitboxIndex < focus->hitboxCount_; ++hitboxIndex)
	{
		Hitbox& hitbox{ focus->hitboxes_[hitboxIndex] };

		// Skip hitbox if it does not have a map or interact/+ mode.
		if (IsHitboxMissingTileModes(hitbox))
		{
			continue;
		}

		EraseOldHitboxData(hitbox);

		int2 hitboxStart{ GetHitboxStart(focus, hitbox) };
		int2 hitboxEnd{ GetHitboxEnd(hitboxStart, hitbox) };

		// Get the top-left tile being overlapped. Find its world position origin. Increment by tile size until the value is beyond the hitbox's x/y end value.
		int2 initialTilePosition{ (hitboxStart.x / TILE_WIDTH) * TILE_WIDTH, (hitboxStart.y / TILE_HEIGHT) * TILE_HEIGHT };

		// Tracks tiles being added to hitbox. Increments after each addition.
		int index{ 0 };

		// Check all tiles under the hitbox and add them to it.
		int2 tilePos{ 0,0 };
		for (tilePos.y = initialTilePosition.y; tilePos.y < hitboxEnd.y; tilePos.y += TILE_HEIGHT)
		{
			for (tilePos.x = initialTilePosition.x; tilePos.x < hitboxEnd.x; tilePos.x += TILE_WIDTH)
			{
				// Get the mapTile under the tilePos. If it has no collision layers (neither an obstacle nor interact tile), skip it.
				const MapTile& mapTile{ mapData_.GetTileAtPosition(tilePos.x, tilePos.y) };
				if (mapTile.combinedCollisionLayer_ == 0)
				{
					continue;
				}

				// If hitbox only needs the map tile it touches, assign map tile and continue checking.
				if (hitbox.mode_ & Hitbox::Mode::Map)
				{
					hitbox.overlappingTiles_[index] = &mapTile;
					++index;
				}

				// Otherwise, hitbox wants to know about a specific interact tile and possibly if it touches with pixel perfect collision.
				else if (mapTile.interactTile_ != nullptr && (hitbox.tileMask_ & mapTile.interactTile_->hitboxes_[0]->tileLayer_))
				{
					int interactHitboxIndex{ -1 };
					if (HasInteractOverlap(focus, hitbox, hitboxStart, hitboxEnd, tilePos, mapTile, interactHitboxIndex))
					{
						hitbox.overlappingTiles_[index] = &mapTile;
						hitbox.contactHitboxes_[index] = interactHitboxIndex;
						++index;
					}
				}

				// Finally, add underlying tile tags to object overlap list. It is the most general way for the object to tell if it touches something.
				// Used mostly by newly-activated states that need to immediately know what their parent object is touching (and have different tile masks than the previous state).				
				int obstacleCollisionLayer{ mapTile[TileType::Obstacle] == nullptr ? 0 : mapTile[TileType::Obstacle]->hitboxes_[0]->tileLayer_ };
				int interactCollisionLayer{ mapTile[TileType::Interact] == nullptr ? 0 : mapTile[TileType::Interact]->hitboxes_[0]->tileLayer_ };
				focus->overlappingTileLayers_ |= (obstacleCollisionLayer | interactCollisionLayer);
			}
		}
	}
}


const bool TileCollisionManager::HasInteractOverlap(const GameObject* focus, const Hitbox& focusHitbox, const int2& focusHitboxStart, const int2& focusHitboxEnd,
	const int2& tilePos, const MapTile& mapTile, int& interactHitboxIndex) const
{
	// Check all hitboxes of the interact tile (usually only the zipline has more than 1).
	for (int tileHitboxIndex{ 0 }; tileHitboxIndex < mapTile[TileType::Interact]->hitboxCount_; ++tileHitboxIndex)
	{
		const Hitbox& tileHitbox{ *(mapTile[TileType::Interact]->hitboxes_[tileHitboxIndex]) };

		int2 tileHitboxStart{ tilePos.x + tileHitbox.offset_.x, tilePos.y + tileHitbox.offset_.y };
		int2 tileHitboxEnd{ tileHitboxStart.x + tileHitbox.size_.x, tileHitboxStart.y + tileHitbox.size_.y };

		// AABB check to make sure interact object and focus hitbox are touching.
		if ((focusHitboxEnd.x > tileHitboxStart.x && tileHitboxEnd.x > focusHitboxStart.x)
			&& (focusHitboxEnd.y > tileHitboxStart.y && tileHitboxEnd.y > focusHitboxStart.y))
		{
			// If normal interact mode, return true once hitbox overlap is confirmed.
			if (focusHitbox.mode_ & Hitbox::Mode::Interact)
			{
				return true;
			}

			// Otherwise, interact+ mode requires a pixel check to ensure the sprites touch.
			else
			{
				int2 contactPoint{ 0, 0 };
				if (pixelChecker_->DoesFocusTouchTile(*focus, focusHitbox, tileHitbox, mapTile, TileType::Interact))
				{
					interactHitboxIndex = tileHitboxIndex; // specific index of the interactTile's hitbox that was touched.
					return true;
				}
			}
		}
	}

	return false;
}	


const bool TileCollisionManager::IsHitboxMissingTileModes(const Hitbox& hitbox) const
{
	return !(hitbox.mode_ & tileModes_);
}


// At most, a hitbox can only touch 4 other tiles since no map / interact hitbox is larger than a single tile.
void TileCollisionManager::EraseOldHitboxData(Hitbox& hitbox) const
{
	for (int index{ 0 }; index < 4; ++index)
	{
		hitbox.overlappingTiles_[index] = nullptr;
		hitbox.contactHitboxes_[index] = -1;
	}
}


// Setup loop variables. Hitbox is constrained by edges of the map.		
const int2 TileCollisionManager::GetHitboxStart(const GameObject* focus, const Hitbox& hitbox) const
{
	return {
		static_cast<int>(max(0.0f, focus->shape_->start_.x + hitbox.offset_.x)),
		static_cast<int>(max(0.0f, focus->shape_->start_.y + hitbox.offset_.y))
	};
}


const int2 TileCollisionManager::GetHitboxEnd(const int2& hitboxStart, const Hitbox & hitbox) const
{
	return{
		static_cast<int>(min(tileSize_.x * TILE_WIDTH, hitboxStart.x + hitbox.size_.x)),
		static_cast<int>(min(tileSize_.y * TILE_HEIGHT, hitboxStart.y + hitbox.size_.y))
	};
}