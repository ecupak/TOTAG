#pragma once

struct GameObjectRewindData;


struct SwingingRopeRewindData
{
	SwingingRopeRewindData();
	~SwingingRopeRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };

	float time_{ 0.0f };

	int direction_{ 0 };

	bool hasPassenger_{ false };
};

