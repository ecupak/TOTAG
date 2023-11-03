#pragma once

class GameObject;
class Knight;
class Scorekeeper;

struct Vector2;
struct KnightRewindData;


// [Credit] Inspired by object pool: https://gameprogrammingpatterns.com/object-pool.html
class KnightList
{
public:
	KnightList() = default;
	KnightList(const KnightList& other) = delete;
	KnightList& operator=(const KnightList& other) = delete;
	virtual ~KnightList();

	void Initialize(const int size);

	void Create(const float2 startTileToCenterOn, Scorekeeper* scorekeeper, const Vector2* playerPosition);
	void Reset();

	void Update(float deltaTime, float gravity);
	void StoreRewindData(KnightRewindData* rewindData);
	void Rewind(KnightRewindData* rewindData);
	void Resume(KnightRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 16 };
	
	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	Knight* knights_{ nullptr };
};
