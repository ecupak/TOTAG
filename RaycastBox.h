#pragma once

#include "GameObject.h"


// [Credit] Abhishek!
// Avoid non-inheritance headers.
class Raycaster;
struct Vector2;


class RaycastBox : public GameObject
{
public:
	RaycastBox();

	~RaycastBox();

	Vector2 ResolveTileCollisions(GameObject* focus, GameObject objects[], int& objectCount, int touchedObjectsIndex[]);


private:
	struct SortData
	{
		int objectIndex{ 0 };
		float distanceAway{ 100.0f };
	};

	Raycaster* raycaster_;
};

