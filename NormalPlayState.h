#pragma once
#include "GameState.h"


namespace Tmpl8
{
	class NormalPlayState : public GameState
	{
	public:
		NormalPlayState(Game& game);
		virtual ~NormalPlayState();


		// Events that happen when state is first assigned.
		void Enter(const bool isRewindEntry = false) override;


		// Check input and return new state if neccessary.
		GameState* HandleInput() override;


		// Apply any state actions that depend on deltaTime.
		void Update(const float deltaTime) override;


		// Events that happen when state is terminated.
		void Exit() override;
	};
}