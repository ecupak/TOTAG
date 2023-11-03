#pragma once


class GameObject;
class Shape;

class MapData;
struct MapTile;


class CollisionFinder
{
public:
	CollisionFinder(const int2& tileSize, const MapData& mapData);

	void FindCollisionsAgainstFocus(GameObject const* focus, const MapTile* collisionList[], int& collisionListSize) const;

private:
	void AdjustBoundingBoxToFocus(GameObject const* focus, Shape& boundingBox) const;

	void FillCollisionList(GameObject const* focus, Shape& boundingBox, const MapTile* collisionList[], int& collisionListSize) const;

	const int2& tileSize_;
	const MapData& mapData_;
};