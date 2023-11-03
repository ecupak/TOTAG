// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "ActorState.h"


const int ActorState::GetStateIndex() const
{
	return stateIndex_;
}