#pragma once


struct GameObjectRewindData
{
	float2 position_{ 0.0f, 0.0f };
	float2 velocity_{ 0.0f, 0.0f };

	int spriteFrame_{ 0 };
	int overlappingTileLayers_{ 0 };

	bool isFlipped_{ false };
	bool isOnstage_{ false };
	bool isActive_{ false };
};