#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class KnightRecoilState : public EnemyState
{
public:
	KnightRecoilState(Knight& knight_, const float2 collisionNormals);
	KnightRecoilState(Knight& knight, EnemyStateRewindData* rewindData);

	void SetHitboxData() override;


	float2 GetRecoilVelocity();


	void Enter(const bool isRewindEntry = false) override;


	EnemyState* HandleInput(const Action& action, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(EnemyStateRewindData* rewindData) override;


private:
	Knight& knight_;
	float2 collisionNormals_{ 0.0f, 0.0f };

	float recoilIntensity_{ 120.0f };

	float maxRecoilTime_{ 0.5f };
	float elapsedRecoilTime_{ 0.0f };

	bool isRecoilActive_{ true };
};




