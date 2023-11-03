// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#pragma once


class GameObject;
class Knight;

struct Vector2;


class KnightPool
{
public:
	KnightPool(const int poolSize, const Vector2* playerPosition);
	~KnightPool();

	Knight* Create(float2 archerPosition);

	void Update(float deltaTime, float gravity);

	void AddActive(GameObject* goList[], int& currentIndex);

	int activeCount_{ 0 };

private:
	const int poolSize_{ 0 };
	const Vector2* playerPosition_{ nullptr };
	Knight* knights_{ nullptr };
};
