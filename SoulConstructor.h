#pragma once

class SoulList;
class ObjectData;
class Scorekeeper;
class Observer;


class SoulConstructor
{
public:
	SoulConstructor();

	const int Construct(SoulList& list, const ObjectData& objectData, Scorekeeper* scorekeeper, Observer* observer);

	Sprite& GetSprite();


private:
	Sprite soulSprite_{ new Surface{"assets/sprites/soul.png"}, true, 1 };
};