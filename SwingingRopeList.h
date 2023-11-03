// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#pragma once


class GameObject;
class SwingingRope;

struct Vector2;
struct SwingingRopeRewindData;


class SwingingRopeList
{
public:
	SwingingRopeList() = default;
	SwingingRopeList(const SwingingRopeList& other) = delete;
	SwingingRopeList& operator=(const SwingingRopeList& other) = delete;
	virtual ~SwingingRopeList();

	void Initialize(const int size);

	SwingingRope* Create(float2 archerPosition);
	void Reset();
	
	void Update(float deltaTime);
	void StoreRewindData(SwingingRopeRewindData* rewindData);
	void Rewind(SwingingRopeRewindData* rewindData);
	void Resume(SwingingRopeRewindData* rewindData);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 14 };

	int capacity_{ 0 };

	int currentPoolIndex_{ 0 };
	SwingingRope* ropes_{ nullptr };
};
