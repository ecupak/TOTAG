#include "precomp.h"
#include "ArcherRewindData.h"

#include "EnemyRewindData.h"
#include "EnemyStateRewindData.h"


ArcherRewindData::ArcherRewindData()
	: enemyRewindData_{new EnemyRewindData }
	, enemyStateRewindData_{ new EnemyStateRewindData }
{	}


ArcherRewindData::~ArcherRewindData()
{
	delete enemyRewindData_;
	delete enemyStateRewindData_;
}