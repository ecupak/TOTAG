#pragma once

class GameObject;
class Collectible;
class Scorekeeper;
class Coin;

struct CoinRewindData;


// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
class CoinList
{
public:
	CoinList() = default;
	CoinList(const CoinList& other) = delete;
	CoinList& operator=(const CoinList& other) = delete;
	virtual ~CoinList();

	void Initialize(const int size);

	Coin* Create(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper);
	void Reset();

	void StoreRewindData(CoinRewindData* rewindData);
	void Rewind(CoinRewindData* rewindData);
	void Resume(CoinRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 17 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	Coin* coins_{ nullptr };
};