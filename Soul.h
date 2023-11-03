#pragma once

class Scorekeeper;
class Subject;

struct SoulRewindData;


class Soul : public GameObject
{
public:
	Soul();
	~Soul();


	void Init(const float2 startTileToCenterOn, Sprite* sprite, Scorekeeper* scorekeeper);


	// Manage rewind data.
	void StoreRewindData(SoulRewindData& rewindData);
	void Rewind(SoulRewindData& rewindData);
	void Resume(SoulRewindData& rewindData);


	void OnObjectCollision(GameObject* object) override;


	Subject* onCollect_{ nullptr };


private:
	Scorekeeper* scorekeeper_;
};