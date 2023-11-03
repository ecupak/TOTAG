#pragma once

class ArcherList;
class ObjectData;
class Scorekeeper;
class ArrowPool;

struct Vector2;


class ArcherConstructor
{
public:
	ArcherConstructor();

	const int Construct(ArcherList& list, const ObjectData& objectData, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool);

private:
	Sprite lockonSprite_{ new Surface{"assets/sprites/archer_lockon_sm.png"}, true, 8, 8 };
};