#pragma once

struct ActorRewindData;


struct EnemyRewindData
{
	EnemyRewindData();
	~EnemyRewindData();

	ActorRewindData* actorRewindData_{ nullptr };

	int currentAction_{ 0 };
	float stateDuration_{ 0.0f };
};

