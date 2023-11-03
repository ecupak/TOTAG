#include "precomp.h"
#include "PlayerRewindData.h"

#include "PlayerStateRewindData.h"
#include "ActorRewindData.h"


PlayerRewindData::PlayerRewindData()
	: playerStateRewindData_{ new PlayerStateRewindData }
	, actorRewindData_{new ActorRewindData }
{	}


PlayerRewindData::~PlayerRewindData()
{
	delete playerStateRewindData_;
	delete actorRewindData_;
}