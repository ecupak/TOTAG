#pragma once

class Scorekeeper;
class Subject;

struct CoinRewindData;


class Coin : public GameObject
{
public:
	Coin();
	~Coin();


	void Init(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper);


	// Manage rewind data.
	void StoreRewindData(CoinRewindData& rewindData);
	void Rewind(CoinRewindData& rewindData);
	void Resume(CoinRewindData& rewindData);


	void OnObjectCollision(GameObject* object) override;


	Subject* onCollect_{ nullptr };


private:
	Scorekeeper* scorekeeper_;
};