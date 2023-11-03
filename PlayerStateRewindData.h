#pragma once


class SwingingRope;


struct PlayerStateRewindData
{
	float2 vFloat2_{ 0.0f, 0.0f };

	SwingingRope* vRopePtr_{ nullptr };

	float vFloatA_{ 0.0f };
	float vFloatB_{ 0.0f };

	int vInt_{ 0 };

	bool vBool_{ false };
};

