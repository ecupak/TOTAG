#pragma once

struct GameObjectRewindData;


struct ActorRewindData
{
	ActorRewindData();
	~ActorRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };

	int currentHp_{ 0 };

	bool isOnGround_{ false };
	bool isAgainstWall_{ false };
};

