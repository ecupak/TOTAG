#include "precomp.h"
#include "Level.h"

#include "Parser.h"
#include "GameObject.h"
#include "TileCollision.h"

#include "LayerData.h"
#include "ObjectData.h"
#include "TilesetData.h"
#include "MapData.h"
#include "Subject.h"
#include "Event.h"


// PUBLIC //

Level::Level()
	: layerData_{ new LayerData{ tilesetSprite_, mapTileCount_, mapTileSize_ } }
	, objectData_{ new ObjectData{mapTileCount_, mapTileSize_} }
	, tilesetData_{ new TilesetData{} }
	, mapData_{ new MapData{tilesetSprite_, mapTileSize_} }
	, onLevelStart_{ new Subject{} }
{	}


Level::~Level()
{
	delete layerData_;
	delete objectData_;
	delete tilesetData_;
	delete mapData_;
	delete onLevelStart_;
}


void Level::LoadLevel(const char* filepath)
{
	Parser parser{};
	char* rawLevelData{ nullptr };

	// Get tile ids from Tiled file, assign collision information to each, and collect into map data.
	GetDataFile(filepath, rawLevelData);
	
	GetMapSize(parser, rawLevelData);
	
	layerData_->ConstructData(parser, rawLevelData);

	objectData_->ConstructData(parser, rawLevelData);

	SetTilemapData();

	SetMapData();
	
	// Release memory.
	delete[] rawLevelData;

	// Start music.
	onLevelStart_->Notify(ObjectClass::Unknown, Event::OnLevelStart);
}


Sprite& Level::GetTilesetSprite()
{
	return tilesetSprite_;
}


const LayerData& Level::GetLayerData() const
{
	return *layerData_;
}


const ObjectData& Level::GetObjectData() const
{
	return *objectData_;
}


const MapData& Level::GetMapData() const
{
	return *mapData_;
}


const int& Level::GetTileCount() const
{
	return mapTileCount_;
}


const int2& Level::GetTileSize() const
{
	return mapTileSize_;
}


// PRIVATE //


// Get full text of data file from Tiled.
void Level::GetDataFile(const char* filepath, char*& rawLevelData)
{
	// [Credit] Bogdan
	rawLevelData = new char[strlen(TextFileRead(filepath).c_str()) + 1];
	strcpy(rawLevelData, TextFileRead(filepath).c_str());
}


// Find tile count in x and y axis.
void Level::GetMapSize(Parser& parser, char* rawLevelData)
{
	parser.ParseLevelData(rawLevelData, mapTileSize_);
	mapTileCount_ = mapTileSize_.x * mapTileSize_.y;

	mapData_->CreateMapTiles();
}


// Assign layer data for the tiles that the player will interact/collide with.
void Level::SetTilemapData()
{	
	tilesetData_->CreateTiles(&(mapData_->GetTileset()));

	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Obstacle),  TileCollisionID::Obstacle);
	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Ledge), TileCollisionID::Ledge);
	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Spike), TileCollisionID::Spike);
	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Ladder), TileCollisionID::Ladder);
	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Zipline), TileCollisionID::ZiplineR);
	AssignTileCollisionDataFor(mapTileCount_, layerData_->GetLayerIds(Layer::Trampoline), TileCollisionID::Trampoline);
}


// Assign each tile's bitflag layer based on the layer-array it is part of.
void Level::AssignTileCollisionDataFor(int tileCount, const int tileIdArray[], int collisionLayerToAssign)
{
	// Increase by 1 because the seen list will always include tile id 0, even if it does not appear in the tileIdArray.
	int* seenTileIds{ new int[tileCount + 1] };

	// Tile id 0 will always have nullptr data.
	int currentIndex{ 0 };
	seenTileIds[currentIndex] = 0;
	++currentIndex;

	// Check all ids in the given array. Skip if the id was already seen.
	for (int index{ 0 }; index < tileCount; ++index)
	{
		int tileId{ tileIdArray[index] };

		bool alreadySeen{ false };

		// Determine if id was already processed.
		for (int seenIdIndex{ 0 }; seenIdIndex < currentIndex; ++seenIdIndex)
		{
			if (tileId < 1 || tileId == seenTileIds[seenIdIndex])
			{
				alreadySeen = true;
				break;
			}
		}

		// Go to next id if it was already seen.
		if (alreadySeen || currentIndex > tileCount)
		{
			continue;
		}

		tilesetData_->AddTileCollisionLayer(tileId, collisionLayerToAssign);

		seenTileIds[currentIndex] = tileId;
		++currentIndex;
	}

	delete[] seenTileIds;
}


// Check parsed ids for each layer to find what tile exists at that location.
void Level::SetMapData()
{	
	Layer obstacleLayers[]{ Layer::Obstacle, Layer::Ledge, Layer::Trampoline };
	Layer interactLayers[]{ Layer::Spike, Layer::Ladder, Layer::Trampoline, Layer::Zipline };

	// Walk through the map, tile by tile. 
	for (int y{ 0 }, index{ 0 }; y < mapData_->GetTileSize().y; ++y)
	{
		for (int x{ 0 }; x < mapData_->GetTileSize().x; ++x, ++index)
		{
			const Tile* obstacleTile{ nullptr };	// Actors collide with obstacle tiles and have their velocity adjusted to avoid penetration.
			const Tile* interactTile{ nullptr };	// Actors do not collide with interact tiles, but perform pixel collision to see if they touch after movement.

			for (const Layer& layer : obstacleLayers)
			{
				if (AssignTile(layer, obstacleTile, index))
				{
					break;
				}
			}

			for (const Layer& layer : interactLayers)
			{
				if (AssignTile(layer, interactTile, index))
				{
					break;
				}
			}

			mapData_->AssignMapTileData(x, y, obstacleTile, interactTile);
		}
	}
}


const bool Level::AssignTile(const Layer layer, const Tile*& tile, const int index)
{
	int tileIndex{ layerData_->GetLayerIds(layer)[index] };
	
	if (tileIndex > 0)
	{
		tile = &(tilesetData_->GetTile(tileIndex));
		return true;
	}

	return false;
}