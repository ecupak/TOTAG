#pragma once

class GameObject;
class SavePoint;

struct SavePointRewindData;


// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
class SavePointList
{
public:
	SavePointList() = default;
	SavePointList(const SavePointList& other) = delete;
	SavePointList& operator=(const SavePointList& other) = delete;
	virtual ~SavePointList();

	void Initialize(const int size);

	SavePoint* Create(const float2 startTileToCenterOn, Sprite* sprite);
	void Reset();

	void Update(const float deltaTime);
	void StoreRewindData(SavePointRewindData* rewindData);
	void Rewind(SavePointRewindData* rewindData);
	void Resume(SavePointRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 11 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	SavePoint* savePoints_{ nullptr };
};