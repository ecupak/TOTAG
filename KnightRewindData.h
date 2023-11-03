#pragma once

struct EnemyRewindData;
struct EnemyStateRewindData;


struct KnightRewindData
{
	KnightRewindData();
	~KnightRewindData();

	EnemyRewindData* enemyRewindData_{ nullptr };
	EnemyStateRewindData* enemyStateRewindData_{ nullptr };

	float elapsedInvulnerableTime_{ 0.0f };

	int stateIndex_{ 0 };

	bool isInvulnerable_{ false };
};

