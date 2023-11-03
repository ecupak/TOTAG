#pragma once

class SavePointList;
class ObjectData;
class Observer;
class ParticleSystem;


class SavePointConstructor
{
public:
	SavePointConstructor();

	const int Construct(SavePointList& list, const ObjectData& objectData, Observer* saveSystem, ParticleSystem particleSystem[]);

private:
	Sprite savePointSprite_{ new Surface{"assets/sprites/save_point.png"}, true, 16, 32 };
};