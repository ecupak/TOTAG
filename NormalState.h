#pragma once


namespace Tmpl8
{
	class NormalState : public GameState
	{
	public:
		NormalState(Game& game);
		~NormalState() override;

		// Events that happen when state is first assigned.
		void Enter(const bool isRewindEntry = false) override;


		// Check input and return new state if neccessary.
		GameState* HandleInput() override;


		// Apply any state actions that depend on deltaTime.
		void Update(const float deltaTime) override;


		// Events that happen when state is terminated.
		void Exit() override;

	private:
		Game& game_;
	};
}