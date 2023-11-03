#pragma once


class GameObject;
class Hitbox;
enum class TileType;

struct Tile;
struct Vector2;
struct MapTile;


class PixelChecker
{
public:
	const bool CheckForPixelCollision(GameObject* const focus, Hitbox& focusHitbox, const Tile& targetTile, 
		const int2& targetPosition, Vector2& entryPoint, Vector2& endPoint, int2& location);

	const bool DoesFocusTouchTile(const GameObject& focus, const Hitbox& focusHitbox, const Hitbox& targetHitbox, const MapTile& mapTile, const TileType tileType);

	const bool DoObjectsTouch(const GameObject& first, const GameObject& second, const int2& overlapStart, const int2& overlapEnd);

private:	
	void PixelChecker::PrintTile(const unsigned int* pixelStart, const int pitch, const int2 start = { 0, 0 }, const int2 end = { 16, 16 });

};

