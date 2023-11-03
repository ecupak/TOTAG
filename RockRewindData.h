#pragma once

struct GameObjectRewindData;


struct RockRewindData
{
	RockRewindData();
	~RockRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };

	float lifetime_{ 0.0f };
};

