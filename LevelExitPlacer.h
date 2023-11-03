#pragma once

class LevelExit;
class ObjectData;


class LevelExitPlacer
{
public:
	LevelExitPlacer();

	void Place(LevelExit& exit, const ObjectData& objectData);
};