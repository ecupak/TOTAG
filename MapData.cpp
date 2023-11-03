#include "precomp.h"
#include "MapData.h"

#include "TilesetData.h"
#include "Hitbox.h"


MapData::MapData(Sprite& tilesetSprite, const int2& mapTileSize)
	: tilesetSprite_{ tilesetSprite }
	, tileSize_{ mapTileSize }
{	}


MapData::~MapData()
{
	delete[] mapTiles_;	
}


void MapData::CreateMapTiles()
{
	pixelSize_ = { tileSize_.x * TILE_WIDTH, tileSize_.y * TILE_HEIGHT };

	delete[] mapTiles_;
	mapTiles_ = new MapTile[tileSize_.x * tileSize_.y];
}


const int2& MapData::GetTileSize() const
{
	return tileSize_;
}


const int2& MapData::GetPixelSize() const
{
	return pixelSize_;
}


Sprite& MapData::GetTileset()
{
	return tilesetSprite_;
}


// Convert a world position to the top-left corner of the tile it's inside.
const MapTile& MapData::GetTileAtPosition(const int xPos, const int yPos) const
{
	return GetTileAtIndex(
		(xPos / TILE_WIDTH),
		(yPos / TILE_HEIGHT)
	);
}


const MapTile& MapData::GetTileAtPosition(const float xPos, const float yPos) const
{
	return GetTileAtIndex(
		static_cast<int>((xPos / TILE_WIDTH)),
		static_cast<int>((yPos / TILE_HEIGHT))
	);
}


const MapTile& MapData::GetTileAtIndex(const int column, const int row) const
{
	return mapTiles_[column + (row * tileSize_.x)];
}


void MapData::AssignMapTileData(const int column, const int row, const Tile* obstacleTile, const Tile* interactTile)
{
	MapTile& mapTile{ mapTiles_[column + (row * tileSize_.x)] };

	mapTile.tilesetSource_ = tilesetSprite_.GetSurface();

	mapTile.position = { column * TILE_WIDTH, row * TILE_HEIGHT };

	mapTile.combinedCollisionLayer_ = 
		(obstacleTile == nullptr 
			? 0 
			: obstacleTile->hitboxes_[0]->tileLayer_)
		| (interactTile == nullptr 
			? 0 
			: interactTile->hitboxes_[0]->tileLayer_);

	mapTile.obstacleTile_ = obstacleTile;
	mapTile.interactTile_ = interactTile;
}