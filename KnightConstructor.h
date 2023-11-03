#pragma once

class KnightList;
class ObjectData;
class Scorekeeper;

struct Vector2;


class KnightConstructor
{
public:
	KnightConstructor();

	const int Construct(KnightList& list, const ObjectData& objectData, Scorekeeper* scorekeeper, const Vector2* playerPosition);
};