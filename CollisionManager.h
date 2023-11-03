#pragma once

#include "GameObject.h"
#include "Vector.h"
#include "Pair.h"


// [Credit] https://github.com/YulyaLesheva/Sweep-And-Prune-algorithm
// Basis for this AABB system.
class CollisionManager
{
	// Everything that is not the player will use AABB. Mainly attack hitboxes.
	// The player will use separating axis theorem (SAT) for collisions.

public:
	CollisionManager(Vector<GameObject*>& pool);

	void CheckCollisions();


private:
	void SortAlongXAxis();
	void FindXOverlaps();
	void NotifyCollisionPairs();
	void ResolveAllCollisions();

	Vector<GameObject*>& pool_;
	Vector<Pair<GameObject*, GameObject*>> xOverlaps_;
};