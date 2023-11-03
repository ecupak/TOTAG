#include "precomp.h"
#include "PixelChecker.h"

#include "GameObject.h"
#include "Vector2.h"
#include "Shape.h"
#include "Hitbox.h"

#include "TilesetData.h"
#include "MapData.h"

#include <iostream>


const bool PixelChecker::DoesFocusTouchTile(const GameObject& focus, const Hitbox& focusHitbox, 
	const Hitbox& targetHitbox, const MapTile& mapTile, const TileType tileType)
{
	// Get world position of the hitboxes.
	int2 focusHBStart{
		static_cast<int>(focus.shape_->start_.x) + focusHitbox.offset_.x,
		static_cast<int>(focus.shape_->start_.y) + focusHitbox.offset_.y };

	int2 targetHBStart{
		mapTile.position.x + targetHitbox.offset_.x,
		mapTile.position.y + targetHitbox.offset_.y };


	// Find overlap rectangle between focus hitbox and target hitbox.
	int2 overlapStart{
		max(focusHBStart.x, targetHBStart.x),
		max(focusHBStart.y, targetHBStart.y)
	};

	int2 overlapEnd{
		min(focusHBStart.x + focusHitbox.size_.x, targetHBStart.x + targetHitbox.size_.x),
		min(focusHBStart.y + focusHitbox.size_.y, targetHBStart.y + targetHitbox.size_.y)
	};


	// Confirm overlap.
	if (overlapEnd.x <= overlapStart.x || overlapEnd.y <= overlapStart.y)
	{
		// No overlap, so no collision.
		return false;
	}


	// Get buffer offset for focus and target.
	int2 offset{ 0, 0 };


	// ... Focus.
	offset.x = overlapStart.x - static_cast<int>(focus.shape_->start_.x);
	offset.y = overlapStart.y - static_cast<int>(focus.shape_->start_.y);
	int focusPitch{ focus.sprite_->GetSurface()->width };
	const uint* focusBuffer{ focus.sprite_->GetCurrentFrameBuffer() + offset.x + (offset.y * focusPitch) };
	int spriteWidth{ focus.sprite_->GetWidth() };

	// ... Retain for pixel check later (if flipped).
	int focusXOffset{ offset.x };


	// ... Target.
	offset.x = overlapStart.x - mapTile.position.x;
	offset.y = overlapStart.y - mapTile.position.y;
	int targetPitch{ mapTile.tilesetSource_->width };
	const uint* targetBuffer{ mapTile[tileType]->startingPixel_ + offset.x + (offset.y * targetPitch) };

	// Loop over the overlap area and if the tile has a non-zero pixel value, check against the player sprite.
	bool isFocusFlipped{ focus.sprite_->IsFlipped() };

	for (int y{ overlapStart.y }; y < overlapEnd.y; ++y)
	{
		for (int x{ 0 }; x < (overlapEnd.x - overlapStart.x); ++x)
		{
			// If the tile has a non-zero pixel at this position and the focus also has a non-zero pixel...			
			if (targetBuffer[x] & 0xFFFFFF)
			{
				if (isFocusFlipped)
				{
					if (targetBuffer[x] & 0xFFFFFF
						// Sample pixels of the horizontally flipped sprite.
						// focusXOffset removed twice (once because it was already added in so it will look correct for right-facing sprite).
						&& focusBuffer[(spriteWidth - 1 - focusXOffset) - x - focusXOffset] & 0xFFFFFF)
					{
						return true;						
					}
				}
				else
				{
					if (targetBuffer[x] & 0xFFFFFF && focusBuffer[x] & 0xFFFFFF)
					{
						return true;						
					}
				}				
			}
		}

		targetBuffer += targetPitch;
		focusBuffer += focusPitch;
	}

	return false;
}


const bool PixelChecker::DoObjectsTouch(const GameObject& first, const GameObject& second, const int2& overlapStart, const int2& overlapEnd)
{
	// Get buffer offset for each object.	
	const GameObject* go[2]{ &first, &second };
	int2 offsets[2]{ {0, 0} };
	int spriteWidths[2]{ 0, 0 };
	int pitches[2]{ 0, 0 };
	uint* buffers[2]{ nullptr, nullptr };
	
	for (int objId{ 0 }; objId < 2; ++objId)
	{
		offsets[objId].x = overlapStart.x - static_cast<int>(go[objId]->shape_->start_.x);
		offsets[objId].y = overlapStart.y - static_cast<int>(go[objId]->shape_->start_.y);

		spriteWidths[objId] = go[objId]->sprite_->GetWidth();

		pitches[objId] = go[objId]->sprite_->GetSurface()->width;
		 
		buffers[objId] = go[objId]->sprite_->GetCurrentFrameBuffer() + offsets[objId].x + (offsets[objId].y * pitches[objId]);
	}

	// Loop over the overlap area and if the first object has a non-zero pixel value, check against the second object.
	bool isFlipped[2]{ go[0]->isFlipped_, go[1]->isFlipped_ };

	for (int y{ overlapStart.y }; y < overlapEnd.y; ++y)
	{
		for (int x{ 0 }; x < (overlapEnd.x - overlapStart.x); ++x)
		{
			// Check first go for a pixel at position.
			bool wasPixelFound{ (!isFlipped[0] && buffers[0][x] & 0xFFFFFF)
				|| (isFlipped[0] && buffers[0][(spriteWidths[0] - 1 - offsets[0].x) - x - offsets[0].x] & 0xFFFFFF)
			};

			// If pixel was found, check second go for a pixel at position.
			if (wasPixelFound && ((!isFlipped[1] && buffers[1][x] & 0xFFFFFF)
				|| (isFlipped[1] && buffers[1][(spriteWidths[1] - 1 - offsets[1].x) - x - offsets[1].x] & 0xFFFFFF)))
			{
				return true;
			}
		}

		buffers[0] += pitches[0];
		buffers[1] += pitches[1];
	}
	
	return false;
}