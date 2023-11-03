#include "precomp.h"
#include "SoulRewindData.h"

#include "GameObjectRewindData.h"


SoulRewindData::SoulRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


SoulRewindData::~SoulRewindData()
{
	delete gameObjectRewindData_;
}