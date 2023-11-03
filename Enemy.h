#pragma once

class Scorekeeper;

enum Action;

struct EnemyRewindData;


class Enemy : public Actor
{
public:
	Enemy(const ObjectClass objectClass, int tileLayer, int tileMask, int objectLayer, int objectMask);
	~Enemy();

	
	void Init(const float2 startTileToCenterOn, const Vector2* playerPosition, Scorekeeper* scorekeeper);


	// Manage rewind data.
	void StoreRewindData(EnemyRewindData* rewindData);
	void Rewind(EnemyRewindData* rewindData);
	void Resume(EnemyRewindData* rewindData);


	void FinishUpdate(float deltaTime, float gravity);


	void Draw(Surface* screen, int x, int y) override;


protected:
	// This generates an input to pass to the current state.
	virtual Action GetEnemyAction(float deltaTime) = 0;

	float stateDuration_{ 0.0f };
	Action currentAction_;

	const Vector2* playerPosition_{ nullptr };

	Scorekeeper* scorekeeper_{ nullptr };
};

