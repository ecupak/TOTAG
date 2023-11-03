#pragma once

class Level;
class MapData;
struct MapTile;
struct Tile;

class GameObject;
class ObjectPooler;

class CollisionFinder;
class Raycaster;
class Hitbox;
class PixelChecker;

struct Vector2;


class TileColllisionManager
{
public:
	TileColllisionManager(const Level& level, const MapData& mapData, const ObjectPooler& objectPooler);
	~TileColllisionManager();

	void ProcessCollisions(const float deltaTime);
		
private:
	// CHECK EACH OBJECT AGAINST FOCUS RAYCAST //

	void SortObjectsByDistance(GameObject* const focus);
	
	void TileColllisionManager::FindEndOfCollisionList();

	void CheckObjectsAgainstFocus(GameObject* const focus, const float deltaTime);

	/*
	const bool CheckForPixelCollision(GameObject* const focus, Hitbox& focusHitbox, const Tile& targetTile, const int2& targetPosition,
		Vector2& entryPoint, Vector2& endPoint, int2& location);
		*/

	// RESOLVE COLLISION //

	void AdjustForObstacleCollision(GameObject* const focus, Vector2& contactNormals, const Vector2& contactNormal, const float timeToContact);

	void SetHitboxOverlaps(GameObject* const focus);

	void NotifyFocusOfObstacleCollision(GameObject* const focus, const Vector2& contactNormals, const float deltaTime);


	const Level& level_;

	const MapData& mapData_;

	const ObjectPooler& objectPooler_;

	const MapTile** collisionList_{ nullptr };
	int collisionListCurrentIndex_{ 0 };

	CollisionFinder* collisionFinder_;

	PixelChecker* pixelChecker_;

	Raycaster* raycaster_;
};

