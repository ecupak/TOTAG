#pragma once


class Parser;
class LayerData;
class ObjectData;
class TilesetData;
class MapData;
class Subject;

struct Tile;

enum class Layer;


class Level
{
public:
	Level();
	~Level();

	void LoadLevel(const char* filepath);

	Sprite& GetTilesetSprite();

	const LayerData& GetLayerData() const;
	
	const ObjectData& GetObjectData() const;

	const MapData& GetMapData() const;

	const int& GetTileCount() const;

	const int2& GetTileSize() const;
	
	Subject* onLevelStart_{ nullptr };


private:
	void GetDataFile(const char* filepath, char*& rawLevelData);

	void GetMapSize(Parser& parser, char* rawLevelData);
	
	void SetTilemapData();
	
	void AssignTileCollisionDataFor(int tileCount, const int tileIdArray[], int collisionLayerToAssign);
	
	void SetMapData();

	const bool AssignTile(const Layer layer, const Tile*& tile, const int index);

	
	int2 mapTileSize_{ 0, 0 };
	int mapTileCount_{ 0 };

	Sprite tilesetSprite_{ new Surface{"assets/tilesets/Revised Desert Ruins Tileset.png"}, true, TILE_WIDTH, TILE_HEIGHT };

	LayerData* layerData_{ nullptr };
	ObjectData* objectData_{ nullptr };
	TilesetData* tilesetData_{ nullptr };
	MapData* mapData_{ nullptr };
};

