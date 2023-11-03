#pragma once

struct GameObjectRewindData;


struct ArrowRewindData
{
	ArrowRewindData();
	~ArrowRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };

	float2 previousPosition_{ 0.0f, 0.0f };

	float lifetime_{ 0.0f };

	bool isStuck_{ false };
};

