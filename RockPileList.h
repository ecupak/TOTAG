#pragma once

class GameObject;
class RockPile;

struct RockPileRewindData;


// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
class RockPileList
{
public:
	RockPileList() = default;
	RockPileList(const RockPileList& other) = delete;
	RockPileList& operator=(const RockPileList& other) = delete;
	virtual ~RockPileList();

	void Initialize(const int size);

	RockPile* Create(const float2 startTileToCenterOn, Sprite* sprite);
	void Reset();

	void StoreRewindData(RockPileRewindData* rewindData);
	void Rewind(RockPileRewindData* rewindData);
	void Resume(RockPileRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 12 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	RockPile* rockPiles_{ nullptr };
};