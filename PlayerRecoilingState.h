#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerRecoilingState : public PlayerState
{
public:
	PlayerRecoilingState(Player& player, const float2 postCollisionNormals);
	PlayerRecoilingState(Player& player, PlayerStateRewindData* rewindData);

	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


private:
	float2 GetRecoilVelocity();

	void AddMissingCollisionMask();

	float2 rawRecoilVelocity_{ 0.0f, 0.0f };
	float2 collisionNormals_{ 0.0f, 0.0f };
	
	float recoilIntensity_{ 120.0f };
	int missingCollisionMask_{ 0 };

	float maxRecoilTime_{ 0.5f };
	float elapsedRecoilTime_{ 0.0f };

	bool isRecoilActive_{ true };
};