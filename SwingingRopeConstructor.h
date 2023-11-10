#pragma once

class SwingingRopeList;
class ObjectData;
class Observer;


class SwingingRopeConstructor
{
public:
	SwingingRopeConstructor();
	SwingingRopeConstructor(const SwingingRopeConstructor& other) = delete;
	SwingingRopeConstructor& operator=(const SwingingRopeConstructor& other) = delete;
	~SwingingRopeConstructor();


	const int Construct(SwingingRopeList& list, const ObjectData& objectData, Observer* observer);

private:
	Sprite* sharedSprite_{ nullptr };
};