#pragma once

class GameObject;
class Camera;
class Player;
class PixelChecker;


// [Credit] https://github.com/YulyaLesheva/Sweep-And-Prune-algorithm
// Basis for this AABB system.

/*
* Only Actor and Prop GameObjects are involved in object collisions.
* The Shape property is used to determine general AABB overlaps.
* Hitboxes are then used to find/confirm the overlap before doing a pixel check.
*/
class ObjectCollisionManager
{
public:
	ObjectCollisionManager(const Camera& camera, GameObject* const activeList[]);
	~ObjectCollisionManager();


	void ProcessCollisions(const int activeCount);


private:
	void GatherOnstageObjects(const int activeCount);

	void SortAlongXAxis();
	
	void FindXOverlaps();	
	
	void FindYOverlaps();
	
	void FindPixelOverlaps();
	
	void NotifyCollisionPairs();

	GameObject* const (*activeList_) { nullptr };
	const Camera& camera_;

	PixelChecker* pixelChecker_;

	// Onstage objects are touching the extended camera. Will get drawn and will update.
	GameObject* (*onstageObjects_) { nullptr };
	int onstageCurrentIndex_{ 0 };

	// Objects that first overlap on the x axis, and then also on the y axis.
	GameObject* (*objectXPairs_) { nullptr };
	int xPairsCurrentIndex_{ 0 };

	GameObject* (*objectOverlaps_) { nullptr };
	int overlapsCurrentIndex_{ 0 };

	// Objects that have at least 1 pixel overlapping.
	GameObject* (*pixelPairs_) { nullptr };
	int pixelCurrentIndex_{ 0 };
};