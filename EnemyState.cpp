#include "precomp.h"
#include "EnemyState.h"

#include "KeyboardManager.h"


const int EnemyState::GetOpposingDirectionsAsValue(const Action& action) const
{
	return (action == Action::Left ? -1 : 0) + (action == Action::Right ? 1 : 0);
}