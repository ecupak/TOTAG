#include "precomp.h"
#include "RockPileRewindData.h"

#include "GameObjectRewindData.h"


RockPileRewindData::RockPileRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


RockPileRewindData::~RockPileRewindData()
{
	delete gameObjectRewindData_;
}