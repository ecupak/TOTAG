#pragma once

struct PlayerStateRewindData;
struct ActorRewindData;


struct PlayerRewindData
{
public:
	PlayerRewindData();
	PlayerRewindData::~PlayerRewindData();

	PlayerStateRewindData* playerStateRewindData_{ nullptr };
	ActorRewindData* actorRewindData_{ nullptr };

	int rockInventory_{ 0 };
	int stateIndex_{ 0 };

	float ladderDelay_{ 0.0f };
	float ziplineDelay_{ 0.0f };
	float ropeDelay_{ 0.0f };
	float throwDelay_{ 0.0f };
	float elapsedInvulnerableTime_{ 0.0f };

	bool canGrabLadder_{ false };
	bool canGrabZipline_{ false };
	bool canGrabRope_{ false };
	bool canThrow_{ false };
	bool isInvulnerable_{ false };
};