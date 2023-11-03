#pragma once

struct GameObjectRewindData;


struct SoulRewindData
{
	SoulRewindData();
	~SoulRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };
};

