#pragma once

class GameObject;
class Arrow;
class Shape;

struct ArrowRewindData;


// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
class ArrowPool
{
public:
	ArrowPool(const int poolSize);
	ArrowPool(const ArrowPool& other) = delete;
	ArrowPool& operator=(const ArrowPool& other) = delete;
	virtual ~ArrowPool();

	
	Arrow* Create(const float2& position, const int archerWidth, const float2& direction, const float speed);
	void Reset();


	void Update(float deltaTime, float gravity);


	// Manage rewind data.
	void StoreRewindData(ArrowRewindData* rewindData);
	void Rewind(ArrowRewindData* rewindData);
	void Resume(ArrowRewindData* rewindData);


	void AddActive(GameObject* goList[], int& currentIndex);

private:
	const int poolSize_{ 0 };
	Arrow* arrows_{ nullptr };
};