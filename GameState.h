#pragma once

class GameState : public State
{
public:
	//GameState(Game& game);
	~GameState() override;


	// Events that happen when state is first assigned to actor.
	void Enter(const bool isRewindEntry = false) override;


	// Return new state based on input (or same state as nullptr).
	virtual GameState* HandleInput();


	// Apply any state actions that depend on deltaTime.
	void Update(const float) override;


	// Events that happen when state is terminated.
	void Exit() override;


protected:
	//Game& game_;
};