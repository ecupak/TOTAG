#include "precomp.h"
#include "RockPileConstructor.h"

#include "RockPile.h"
#include "RockPileList.h"
#include "ObjectData.h"
#include "Observer.h"
#include "Subject.h"


RockPileConstructor::RockPileConstructor() {}


const int RockPileConstructor::Construct(RockPileList& list, const ObjectData& objectData, Observer* observer)
{
	int objectId{ list.GetObjectId() };
	const int* objectIds_{ objectData.GetObjectIds() };
	const int2& mapSize{ objectData.GetMapTileSize() };

	int2 positions[200]{ {0, 0} };
	int positionsCurrentIndex_{ 0 };

	// Check each tile of the level.
	for (int y{ 0 }; y < mapSize.y; ++y)
	{
		for (int x{ 0 }; x < mapSize.x; ++x)
		{
			if (objectIds_[x + (y * mapSize.x)] == objectId)
			{
				// Store positions and increase count of objects found.
				positions[positionsCurrentIndex_++] = { x, y };
			}
		}
	}

	// Set capacity of list.
	list.Initialize(positionsCurrentIndex_);

	// Create each object in the list.
	for (int index{ 0 }; index < positionsCurrentIndex_; ++index)
	{
		RockPile* newRockPile{ list.Create({ 1.0f * positions[index].x * TILE_WIDTH, 1.0f * positions[index].y * TILE_HEIGHT }, &rockPileSprite_) };

		if (newRockPile != nullptr)
		{
			newRockPile->onCollect_->AddObserver(observer);
		}
	}

	return positionsCurrentIndex_;
}