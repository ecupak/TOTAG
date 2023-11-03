#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerJumpingState : public PlayerState
{
public:
	PlayerJumpingState(Player& player, bool isPlayerChoice);
	PlayerJumpingState(Player& player, PlayerStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


private:		
	float maxJumpTime_{ 0.16f };
	float jumpTimer_{ 0.0f };
		
	bool isPlayerChoice_{ true };	
};