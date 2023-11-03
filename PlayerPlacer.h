#pragma once

class Player;
class ObjectData;


class PlayerPlacer
{
public:
	PlayerPlacer() = default;

	void Place(Player& player, const ObjectData& objectData);
};