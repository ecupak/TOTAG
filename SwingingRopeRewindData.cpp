#include "precomp.h"
#include "SwingingRopeRewindData.h"

#include "GameObjectRewindData.h"


SwingingRopeRewindData::SwingingRopeRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


SwingingRopeRewindData::~SwingingRopeRewindData()
{
	delete gameObjectRewindData_;
}