#pragma once

class Enemy;


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class EnemyState : public ActorState
{
protected :
	EnemyState() {}

	const int GetOpposingDirectionsAsValue(const Action& action) const;
};
