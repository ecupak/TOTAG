#pragma once

#include "ObjectPooler.h"


// Forward declares.
class Raycaster;
class GameObject;
class Level;
struct Shape;
struct CollisionResult;


class HotspotCollisionManager
{
public:
	HotspotCollisionManager(ObjectPooler& tileObjectPool, Level& level);


	void ProcessCollisions();


private:
	void GetCollisionsFor(GameObject* focus, CollisionResult& collisionResult);
	void CreateBoundingBox(GameObject* focus, Shape& boundingBox);
	void FindTileCollisions(GameObject* focus, Shape& boundingBox, Vector<GameObject>& objects);


	void NotifyCollisionsFor(GameObject* focus, CollisionResult& collisionResult);


	void NotifyCornersFor(GameObject* focus);


	const int& GetCollisionLayerOfTileAtPosition(const int2& position) const;
	const int& GetCollisionLayerOfTileAtPosition(const int x, const int y) const;
	const int& GetTileCollisionIdAtIndex(const int x, const int y) const;
	const int GetTileRowAtPosition(const int y) const;
	const int GetTileColumnAtPosition(const int x) const;

	int2 GetCornerPosition(int cornerIndex, GameObject* focus);

	const bool IsObjectTouchingCollisionId(GameObject* object, const int collisionId, const float2& offset);
	const bool IsCornerTouchingCollisionId(GameObject* object, const int collisionId, float2& corner, const float2& offset);


	void PrintTile(const unsigned int* pixelStart, int tileOffset);


	Level& level_;
	ObjectPooler& tileObjectPool_;
	Raycaster* raycaster_;

	struct SortData
	{
		GameObject* object;
		float distanceAway{ 100.0f };
	};
	Vector<SortData> results_;
};

