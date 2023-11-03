#pragma once

struct GameObjectRewindData;


struct CoinRewindData
{
	CoinRewindData();
	~CoinRewindData();

	GameObjectRewindData* gameObjectRewindData_{ nullptr };
};

