#include "precomp.h"
#include "ActorRewindData.h"

#include "GameObjectRewindData.h"


ActorRewindData::ActorRewindData()
	: gameObjectRewindData_{ new GameObjectRewindData }
{	}


ActorRewindData::~ActorRewindData()
{
	delete gameObjectRewindData_;
}