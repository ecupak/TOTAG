#include "precomp.h"
#include "SavePointConstructor.h"

#include "SavePoint.h"
#include "SavePointList.h"
#include "ObjectData.h"
#include "Observer.h"
#include "Subject.h"

#include "ParticleSystem.h"


SavePointConstructor::SavePointConstructor() {}


const int SavePointConstructor::Construct(SavePointList& list, const ObjectData& objectData, Observer* saveSystem, ParticleSystem particleSystem[])
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
		SavePoint* newSavePoint{ list.Create({ 1.0f * positions[index].x * TILE_WIDTH, 1.0f * positions[index].y * TILE_HEIGHT }, &savePointSprite_) };

		if (newSavePoint)
		{
			newSavePoint->onPlayerTouch_->AddObserver(saveSystem);
			newSavePoint->onPlayerTouch_->AddObserver(particleSystem);

			particleSystem[index].Initialize({ 1.0f * positions[index].x * TILE_WIDTH, 1.0f * positions[index].y * TILE_HEIGHT }, 50, 10);
		}
	}

	return positionsCurrentIndex_;
}