#pragma once
#include "GameState.h"


namespace Tmpl8
{
	class RewindPlayState : public GameState
	{
	public:
		RewindPlayState(Game& game);
		virtual ~RewindPlayState();


		// Events that happen when state is first assigned.
		void Enter(const bool isRewindEntry = false) override;


		// Check input and return new state if neccessary.
		GameState* HandleInput() override;


		// Apply any state actions that depend on deltaTime.
		void Update(const float deltaTime) override;


		// Events that happen when state is terminated.
		void Exit() override;


	private:
		int rewindDirection_{ 0 };
	};
}