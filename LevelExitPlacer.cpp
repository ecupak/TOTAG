#include "precomp.h"
#include "LevelExitPlacer.h"

#include "ObjectData.h"
#include "LevelExit.h"


LevelExitPlacer::LevelExitPlacer() {}


void LevelExitPlacer::Place(LevelExit& levelExit, const ObjectData& objectData)
{
	int objectId{ 5 };
	const int* objectIds_{ objectData.GetObjectIds() };
	const int2& mapSize{ objectData.GetMapTileSize() };

	// Check each tile of the level.
	for (int y{ 0 }; y < mapSize.y; ++y)
	{
		for (int x{ 0 }; x < mapSize.x; ++x)
		{
			// Create object.
			if (objectIds_[x + (y * mapSize.x)] == objectId)
			{
				levelExit.Place({ 1.0f * x * TILE_WIDTH, 1.0f * y * TILE_HEIGHT });
			}
		}
	}
}