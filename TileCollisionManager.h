#pragma once

class GameObject;
class MapData;
class Hitbox;
struct MapTile;

class PixelChecker;


/*
* Only Actor GameObjects are involved in tile collisions.
* Each hitbox in map mode will store the map tiles it overlaps.
* Each hitbox in interact mode will store the map tile and specific hitbox of the interact tile it overlaps
* The Actor also stores the tile tags of all overlapping map tiles in an aggregated overlappingTileLayers variable.
*/
class TileCollisionManager
{
public:
	TileCollisionManager(const int2& tileSize, const MapData& mapData, GameObject* const activeList[]);
	~TileCollisionManager();

	void ProcessCollisions(const int activeCount);

	void SetHitboxOverlaps(GameObject* const focus);

private:
	const bool IsHitboxMissingTileModes(const Hitbox& hitbox) const;
	void EraseOldHitboxData(Hitbox& hitbox) const;
	const int2 GetHitboxStart(const GameObject* focus, const Hitbox& hitbox) const;	
	const int2 GetHitboxEnd(const int2& hitboxStart, const Hitbox& hitbox) const;
	const bool HasInteractOverlap(const GameObject* focus, const Hitbox& focusHitbox, const int2& focusHitboxStart, const int2& focusHitboxEnd, 
		const int2& tilePos, const MapTile& mapTile, int& interactHitboxIndex) const;

	const int2& tileSize_;
	const MapData& mapData_;
	GameObject* const (*activeList_) { nullptr };
	const int tileModes_;

	PixelChecker* pixelChecker_{ nullptr };
};

