#include "precomp.h"
#include "ArcherConstructor.h"

#include "ArcherList.h"
#include "ObjectData.h"


ArcherConstructor::ArcherConstructor() {}


const int ArcherConstructor::Construct(ArcherList& list, const ObjectData& objectData, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool)
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
		list.Create({ 1.0f * positions[index].x * TILE_WIDTH, 1.0f * positions[index].y * TILE_HEIGHT}, &lockonSprite_, scorekeeper, playerPosition, arrowPool);
	}

	return positionsCurrentIndex_;
}