#include "precomp.h"
#include "TilesetData.h"

#include "Hitbox.h"
#include "TileCollision.h"


TilesetData::TilesetData() {}


TilesetData::~TilesetData()
{
	DeleteHitboxes();

	delete[] tiles_;
}


void TilesetData::DeleteHitboxes()
{
	for (int tileIndex{ 0 }; tileIndex < tileCount_; ++tileIndex)
	{
		for (int hitboxIndex{ 0 }; hitboxIndex < 4; ++hitboxIndex)
		{
			delete tiles_[tileIndex].hitboxes_[hitboxIndex];
		}
	}
}


void TilesetData::CreateTiles(Sprite* const tilesetSprite)
{
	// Create array of tiles and fill with tile data.
	DeleteHitboxes();
	
	// Avoid buffer overruns. This is just a poor man's assert.
	tileCount_ = tilesetSprite->GetRows() * tilesetSprite->GetColumns();
	if (tileCount_ < 512)
	{
		return;
	}
	
	// Make new tiles.
	delete[] tiles_;
	tiles_ = new Tile[tileCount_];

	// Assign all tiles their starting coordinates and the default hitbox.
	{
		int tileIndex{ 0 };
		for (int y{ 0 }; y < tilesetSprite->GetRows(); ++y)
		{
			for (int x{ 0 }; x < tilesetSprite->GetColumns(); ++x)
			{
				Tile& tile{ tiles_[tileIndex] };

				tile.id_ = tileIndex;
				tile.hitboxes_[0] = new Hitbox{ TILE_WIDTH, 0, 0, TILE_WIDTH, TILE_HEIGHT, Hitbox::Mode::None };
				tile.hitboxCount_ = 1;
				tile.startingPixel_ = tilesetSprite->GetBuffer() + (x * TILE_WIDTH) + (y * TILE_HEIGHT * tilesetSprite->GetSurface()->width);

				++tileIndex;
			}
		}
	}

	// Assign unique hitboxes to select tiles.

	// Ropes
	{
		// ... These are ropes that cut vertically through the entire tile.
		int tileIds[]{ 20, 21, 22, 23, 52, 54, 84 };
		for (int tileId : tileIds)
		{
			delete tiles_[tileId].hitboxes_[0];
			tiles_[tileId].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 5, 0, 5, TILE_HEIGHT, Hitbox::Mode::None };
			tiles_[tileId].hitboxCount_ = 1;
		}

		// ... has different shape than the other full-length ropes.
		delete tiles_[55].hitboxes_[0];
		tiles_[55].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 6, 0, 4, TILE_HEIGHT, Hitbox::Mode::None };
		tiles_[55].hitboxCount_ = 1;

		// ... These are ropes that end before the bottom of the tile.
		delete tiles_[86].hitboxes_[0];
		tiles_[86].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 5, 0, 5, 7, Hitbox::Mode::None };
		tiles_[86].hitboxCount_ = 1;

		delete tiles_[87].hitboxes_[0];
		tiles_[87].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 7, 0, 4, 13, Hitbox::Mode::None };
		tiles_[87].hitboxCount_ = 1;

		delete tiles_[116].hitboxes_[0];
		tiles_[116].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 5, 0, 4, 13, Hitbox::Mode::None };
		tiles_[116].hitboxCount_ = 1;
	}


	// Tiles with image in only the upper half.
	{
		int tileIds[]{ 140, 141, 142, 143, 144, 183 };
		for (int tileId : tileIds)
		{
			delete tiles_[tileId].hitboxes_[0];
			tiles_[tileId].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 0, 0, TILE_WIDTH, 7, Hitbox::Mode::None };
			tiles_[tileId].hitboxCount_ = 1;
		}
	}


	// Zipline tiles.
	delete tiles_[393].hitboxes_[0];
	tiles_[393].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 0, 7, 5, 3, Hitbox::Mode::None };
	tiles_[393].hitboxes_[1] = new Hitbox{ TILE_WIDTH, 2, 10, 7, 2, Hitbox::Mode::None };
	tiles_[393].hitboxes_[2] = new Hitbox{ TILE_WIDTH, 6, 12, 7, 2, Hitbox::Mode::None };
	tiles_[393].hitboxes_[3] = new Hitbox{ TILE_WIDTH, 10, 14, 6, 2, Hitbox::Mode::None };
	tiles_[393].hitboxCount_ = 4;

	delete tiles_[426].hitboxes_[0];
	tiles_[426].hitboxes_[0] = new Hitbox{ TILE_WIDTH, 0, 0, 5, 2, Hitbox::Mode::None };
	tiles_[426].hitboxes_[1] = new Hitbox{ TILE_WIDTH, 2, 2, 7, 2, Hitbox::Mode::None };
	tiles_[426].hitboxes_[2] = new Hitbox{ TILE_WIDTH, 6, 4, 7, 2, Hitbox::Mode::None };
	tiles_[426].hitboxes_[3] = new Hitbox{ TILE_WIDTH, 10, 6, 6, 3, Hitbox::Mode::None };
	tiles_[426].hitboxCount_ = 4;
}


void TilesetData::AddTileCollisionLayer(const int tileId, const int collisionLayer)
{
	tiles_[tileId].hitboxes_[0]->tileLayer_ |= collisionLayer;

	// If a tile is both an obstacle and a ledge, turn it into a cliff.
	// Cliffs can be climbed through, but are otherwise solid to all other player states.
	if (tiles_[tileId].hitboxes_[0]->tileLayer_ == (TileCollisionID::Obstacle | TileCollisionID::Ledge))
	{
		tiles_[tileId].hitboxes_[0]->tileLayer_ = TileCollisionID::Cliff;
	}
}


const Tile& TilesetData::GetTile(const int tileId) const
{
	return tiles_[tileId];
}