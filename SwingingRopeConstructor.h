#pragma once

class SwingingRopeList;
class ObjectData;
class Observer;


class SwingingRopeConstructor
{
public:
	SwingingRopeConstructor();

	const int Construct(SwingingRopeList& list, const ObjectData& objectData, Observer* observer);

private:
	Sprite sharedSprite_{ new Surface{static_cast<int>(SwingingRope::GetSharedSurfaceSize().x), static_cast<int>(SwingingRope::GetSharedSurfaceSize().y)}, 1, 1};
};