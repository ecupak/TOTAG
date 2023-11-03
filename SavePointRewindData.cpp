#include "precomp.h"
#include "SavePointRewindData.h"

#include "GameObjectRewindData.h"


SavePointRewindData::SavePointRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


SavePointRewindData::~SavePointRewindData()
{
	delete gameObjectRewindData_;
}