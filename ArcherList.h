#pragma once

class GameObject;
class Archer;
class ArrowPool;
class Scorekeeper;

struct Vector2;
struct ArcherRewindData;


 // [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
class ArcherList
{
public:
	ArcherList() = default;
	ArcherList(const ArcherList& other) = delete;
	ArcherList& operator=(const ArcherList& other) = delete;
	virtual ~ArcherList();

	void Initialize(const int size);

	void Create(const float2 startTileToCenterOn, Sprite* lockonSprite, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool);
	void Reset();

	void Update(float deltaTime, float gravity);
	void StoreRewindData(ArcherRewindData* rewindData);
	void Rewind(ArcherRewindData* rewindData);
	void Resume(ArcherRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

	const int Size() const;


private:
	const int objectId_{ 15 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	Archer* archers_{ nullptr };
};
