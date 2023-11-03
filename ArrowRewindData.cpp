#include "precomp.h"
#include "ArrowRewindData.h"

#include "GameObjectRewindData.h"


ArrowRewindData::ArrowRewindData()
	: gameObjectRewindData_{new GameObjectRewindData }
{	}


ArrowRewindData::~ArrowRewindData()
{
	delete gameObjectRewindData_;
}