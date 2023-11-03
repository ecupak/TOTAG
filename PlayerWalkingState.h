#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerWalkingState : public PlayerState
{
public:
	PlayerWalkingState(Player& player);
	PlayerWalkingState(Player& player, PlayerStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


private:
	const float frameTime_{ 0.2f };
	float elapsedFrameTime_{ 0.0f };
};

