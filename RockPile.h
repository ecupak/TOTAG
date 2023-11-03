#pragma once

class Subject;

struct RockPileRewindData;


class RockPile : public GameObject
{
public:
	RockPile();
	~RockPile();

	void Init(const float2 startTileToCenterOn, Sprite* sprite);


	void Update(RockPileRewindData& rewindData);


	// Manage rewind data.
	void StoreRewindData(RockPileRewindData& rewindData);
	void Rewind(RockPileRewindData& rewindData);
	void Resume(RockPileRewindData& rewindData);


	void OnObjectCollision(GameObject* object) override;


	Subject* onCollect_{ nullptr };
};

