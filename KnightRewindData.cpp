#include "precomp.h"
#include "KnightRewindData.h"

#include "EnemyRewindData.h"
#include "EnemyStateRewindData.h"


KnightRewindData::KnightRewindData()
	: enemyRewindData_{ new EnemyRewindData }
	, enemyStateRewindData_{ new EnemyStateRewindData }
{	}


KnightRewindData::~KnightRewindData()
{
	delete enemyRewindData_;
	delete enemyStateRewindData_;
}