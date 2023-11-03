#pragma once

// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class State
{
public:
	virtual ~State() { }


	// Events that happen when state is first assigned to actor.
	virtual void Enter(const bool isRewindEntry = false) = 0;


	// Apply any state actions that depend on deltaTime.
	virtual void Update(const float deltaTime) = 0;


	// Events that happen when state is terminated.
	virtual void Exit() = 0;
};