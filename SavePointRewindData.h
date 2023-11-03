#pragma once

struct GameObjectRewindData;


struct SavePointRewindData
{
	SavePointRewindData();
	~SavePointRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };

	bool isEnabled_{ false };
};