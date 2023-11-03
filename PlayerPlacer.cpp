#include "precomp.h"
#include "PlayerPlacer.h"

#include "ObjectData.h"
#include "Player.h"


void PlayerPlacer::Place(Player& player, const ObjectData& objectData)
{
	int objectId{ 13 };
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
				player.Place({ 1.0f * x * TILE_WIDTH, 1.0f * y * TILE_HEIGHT });
			}
		}
	}
}