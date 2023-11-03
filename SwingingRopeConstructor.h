#pragma once

class SwingingRopeList;
class ObjectData;
class Observer;


class SwingingRopeConstructor
{
public:
	SwingingRopeConstructor();

	const int Construct(SwingingRopeList& list, const ObjectData& objectData, Observer* observer);
};