#include "precomp.h"
#include "EnemyRewindData.h"

#include "ActorRewindData.h"


EnemyRewindData::EnemyRewindData()
	: actorRewindData_{ new ActorRewindData }
{	}


EnemyRewindData::~EnemyRewindData()
{
	delete actorRewindData_;
}