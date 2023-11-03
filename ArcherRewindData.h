#pragma once

struct EnemyRewindData;
struct EnemyStateRewindData;


struct ArcherRewindData
{
	ArcherRewindData();
	~ArcherRewindData();

	EnemyRewindData* enemyRewindData_{ nullptr };
	EnemyStateRewindData* enemyStateRewindData_{ nullptr };

	float attackDelay_{ 0.0f };

	int lockonSpriteFrame_{ 0 };
	int stateIndex_{ 0 };
};

