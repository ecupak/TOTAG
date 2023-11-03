#pragma once

class GameObject;
class Rock;

struct RockRewindData;


// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
class RockPool
{
public:
	RockPool(const int poolSize);
	RockPool(const RockPool& other) = delete;
	RockPool& operator=(const RockPool& other) = delete;
	virtual ~RockPool();


	Rock* Create(const float2 position, const float2 velocity);
	void Reset();


	void Update(float deltaTime, float gravity);


	void StoreRewindData(RockRewindData* rewindData);
	void Rewind(RockRewindData* rewindData);
	void Resume(RockRewindData* rewindData);


	void AddActive(GameObject* goList[], int& currentIndex);


private:
	const int poolSize_{ 0 };

	Rock* rocks_{ nullptr };

	Sprite rockSprite_{ new Surface{"assets/sprites/rock.png"}, true, 1 };
};