#pragma once

class GameObject;
class Collectible;
class Scorekeeper;
class Soul;

struct SoulRewindData;


// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
class SoulList
{
public:
	SoulList() = default;
	SoulList(const SoulList& other) = delete;
	SoulList& operator=(const SoulList& other) = delete;
	virtual ~SoulList();

	void Initialize(const int size);

	Soul* Create(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper);
	void Reset();

	void StoreRewindData(SoulRewindData* rewindData);
	void Rewind(SoulRewindData* rewindData, const bool isFromSaveFile = false);
	void Resume(SoulRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 10 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	Soul* souls_{ nullptr };
};