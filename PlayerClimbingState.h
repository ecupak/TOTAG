#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerClimbingState : public PlayerState
{
public:
	PlayerClimbingState(Player& player, const int2& ropePosition);
	PlayerClimbingState(Player& player, PlayerStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


private:
	float2 ladderJumpImpulse_{ 60.0f, -50.0f };

	const float ropeCenterX_{ 0.0f };

	float grabTimer_{ 0.0f };
	float grabSpeed_{ 8.0f };

	bool doesPlayerHaveControl_{ false };
};