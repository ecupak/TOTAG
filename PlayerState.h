#pragma once

class Player;


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerState : public ActorState
{
protected:
	PlayerState(Player& player);


	// Helper function for states to get an int value (-1, 0, 1) representing the result of 2 opposing directions that can be pressed.
	const int GetOpposingDirectionsAsValue(const bool directionA, const bool directionB);

	Player& player_;
};