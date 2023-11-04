#pragma once

class GameObject;


class ObjectPooler
{
public:
	ObjectPooler();

	void Add(GameObject* go);

	void Remove(GameObject* go);

	const int GetSize() const;

	GameObject* const operator[](const int index) const;

private:
	GameObject* (*pool_);
	int currentIndex_{ 0 };
};