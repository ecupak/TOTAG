#pragma once

class Subject;
class SavePointParticles;

struct SavePointRewindData;


class SavePoint : public GameObject
{
public:
	SavePoint();
	~SavePoint();


	void Init(const float2 startTileToCenterOn, Sprite* sprite);


	// Manage rewind data.
	void StoreRewindData(SavePointRewindData& rewindData);
	void Rewind(SavePointRewindData& rewindData);
	void Resume(SavePointRewindData& rewindData);


	void OnObjectCollision(GameObject* object) override;

	void Draw(Surface* screen, int x, int y) override;

	Subject* onPlayerTouch_{ nullptr };


private:
	bool isEnabled_{ false };
};