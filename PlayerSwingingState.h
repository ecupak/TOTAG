#pragma once

class GameObject;
class SwingingRope;


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerSwingingState : public PlayerState
{
public:
	PlayerSwingingState(Player& player, GameObject* rope);
	PlayerSwingingState(Player& player, PlayerStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


private:
	float2 launchImpulse_{ 200.0f, 150.0f };
	float2 previousPosition_{ 0.0f, 0.0f };
	
	SwingingRope* rope_;

	float timeOnSegment_{ 0.0f };
	float rappelSpeed_{ 10.0f };
	
	int attachmentPoint_{ 0 };	

	bool canJump_{ false };
};
