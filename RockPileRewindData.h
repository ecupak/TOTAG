#pragma once

struct GameObjectRewindData;


struct RockPileRewindData
{
	RockPileRewindData();
	~RockPileRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };
};

