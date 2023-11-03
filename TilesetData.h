#pragma once

class Hitbox;


struct Tile
{
	int id_{ 0 };
	Hitbox* hitboxes_[4]{ nullptr, nullptr, nullptr, nullptr };
	int hitboxCount_{ 0 };
	const unsigned int* startingPixel_{ nullptr };
};


class TilesetData
{
public:
	TilesetData();
	~TilesetData();

	void CreateTiles(Sprite* const tilesetImage);

	const Tile& GetTile(const int tileId) const;

	void AddTileCollisionLayer(const int tileId, const int collisionLayer);


private:
	void DeleteHitboxes();

	// Set of tiles.
	Tile* tiles_{ nullptr };
	int tileCount_{ 0 };
};