#pragma once

class MapData;
struct MapTile;
struct Tile;

class GameObject;
class CollisionFinder;
class Raycaster;
class Hitbox;

struct Vector2;


class ObstacleCollisionManager
{
public:
	ObstacleCollisionManager(const int2& tileSize, const MapData& mapData, GameObject* const activeList[]);
	~ObstacleCollisionManager();

	void ProcessCollisions(const int activeCount);
		
private:
	// CHECK EACH OBJECT AGAINST FOCUS RAYCAST //

	void SortObjectsByDistance(GameObject* const focus);
	
	void CheckObjectsAgainstFocus(GameObject* const focus);

	/*
	const bool CheckForPixelCollision(GameObject* const focus, Hitbox& focusHitbox, const Tile& targetTile, const int2& targetPosition,
		Vector2& entryPoint, Vector2& endPoint, int2& location);
		*/

	// RESOLVE COLLISION //

	void AdjustForObstacleCollision(GameObject* const focus, Vector2& contactNormals, const Vector2& contactNormal, const float timeToContact);

	void NotifyFocusOfObstacleCollision(GameObject* const focus, const Vector2& contactNormals);


	int tileModes_{ 0b0 };

	const MapData& mapData_;

	GameObject* const (*activeList_) { nullptr };

	const MapTile* (*collisionList_){ nullptr };

	int collisionListCurrentIndex_{ 0 };

	const CollisionFinder* collisionFinder_;

	const Raycaster* raycaster_;

};

