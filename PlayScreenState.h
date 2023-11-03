#pragma once
#include "GameState.h"


namespace Tmpl8
{
	class PlayScreenState : public GameState
	{
	public:
		PlayScreenState(const bool isFromSaveFile, Game& game);
		~PlayScreenState() override;


		// Events that happen when state is first assigned.
		void Enter(const bool isRewindEntry = false) override;


		// Check input and return new state if neccessary.
		GameState* HandleInput() override;


		// Apply any state actions that depend on deltaTime.
		void Update(const float deltaTime) override;


		// Events that happen when state is terminated.
		void Exit() override;

	private:
		void ChangeState(GameState* newState);

		GameState* state_{ nullptr };

		bool isFromSaveFile_{ false };

		Game& game_;
	};
}