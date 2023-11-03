#pragma once

namespace Tmpl8
{
	class EndScreenState : public GameState
	{
	public:
		EndScreenState(const bool isVictoryScreen, Game& game);
		virtual ~EndScreenState();


		// Events that happen when state is first assigned.
		void Enter(const bool isRewindEntry = false) override;


		// Check input and return new state if neccessary.
		GameState* HandleInput() override;


		// Apply any state actions that depend on deltaTime.
		void Update(const float deltaTime) override;


		// Events that happen when state is terminated.
		void Exit() override;


	private:
		GameState* state_{ nullptr };

		bool isVictoryScreen_{ false };

		int cursorIndex_{ 0 };

		Game& game_;
	};
}