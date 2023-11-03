#include "precomp.h"
#include "CoinRewindData.h"

#include "GameObjectRewindData.h"


CoinRewindData::CoinRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


CoinRewindData::~CoinRewindData()
{
	delete gameObjectRewindData_;
}