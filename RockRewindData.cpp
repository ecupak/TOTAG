#include "precomp.h"
#include "RockRewindData.h"

#include "GameObjectRewindData.h"


RockRewindData::RockRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


RockRewindData::~RockRewindData()
{
	delete gameObjectRewindData_;
}