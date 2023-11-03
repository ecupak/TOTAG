// [Credit] Object pool: https://gameprogrammingpatterns.com/object-pool.html
#pragma once


class GameObject;
class Archer;
class ArrowPool;

struct Vector2;


class ArcherPool
{
public:
	ArcherPool(const Vector2* playerPosition, ArrowPool* arrowPool);
	~ArcherPool();

	void Reserve(const int poolSize);

	Archer* Create(float2 archerPosition);

	void Update(float deltaTime, float gravity);

	void AddActive(GameObject* goList[], int& currentIndex);

	const int GetObjectId() const;

private:
	const int objectId_{ 15 };

	int poolSize_{ 0 };
	const Vector2* playerPosition_{ nullptr };
	ArrowPool* arrowPool_{ nullptr };
	Archer* archers_{ nullptr };
};
