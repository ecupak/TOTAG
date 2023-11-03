#pragma once

class CoinList;
class ObjectData;
class Scorekeeper;
class Observer;


class CoinConstructor
{
public:
	CoinConstructor();

	const int Construct(CoinList& list, const ObjectData& objectData, Scorekeeper* scorekeeper, Observer* observer);

	Sprite& CoinConstructor::GetSprite();


private:
	Sprite coinSprite_{ new Surface{"assets/sprites/coin.png"}, true, 1 };
};