#pragma once

struct Tile;

enum class TileType
{
	Obstacle,
	Interact,
	HighDoodad,
	MidDoodad,
	LowDoodad,
	NearBG,
	MidBG,
	FarBG,	
};


struct MapTile
{
	int2 position{ 0, 0 };
	int combinedCollisionLayer_{ 0 };
	Surface* tilesetSource_{ nullptr };

	const Tile* obstacleTile_{ nullptr };
	const Tile* interactTile_{ nullptr };

	const Tile* const operator[](const TileType tileType) const
	{
		switch (tileType)
		{
		case TileType::Obstacle:
			return obstacleTile_;
		case TileType::Interact:
			return interactTile_;
		default:
			return nullptr;
		}
	}
};


class MapData
{
public:
	MapData(Sprite& tilesetSprite, const int2& mapTileSize);
	~MapData();

	void CreateMapTiles();
	
	const int2& GetTileSize() const;
	const int2& GetPixelSize() const;

	Sprite& GetTileset();

	const MapTile& GetTileAtPosition(const int xPos, const int yPos) const;
	const MapTile& GetTileAtPosition(const float xPos, const float yPos) const;
	const MapTile& GetTileAtIndex(const int column, const int row) const;

	void AssignMapTileData(const int column, const int row, const Tile* obstacleTile, const Tile* interactTile);


private:
	Sprite& tilesetSprite_;

	MapTile* mapTiles_{ nullptr };
	const int2& tileSize_;
	int2 pixelSize_{ 0, 0 };
};