#pragma once

class RockPileList;
class ObjectData;
class Observer;


class RockPileConstructor
{
public:
	RockPileConstructor();

	const int Construct(RockPileList& list, const ObjectData& objectData, Observer* observer);

private:
	Sprite rockPileSprite_{ new Surface{"assets/sprites/rockpile.png"}, true, 1 };
};