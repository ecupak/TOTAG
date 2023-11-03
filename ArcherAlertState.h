#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class ArcherAlertState : public EnemyState
{
public:
	ArcherAlertState(Archer& archer);
	ArcherAlertState(Archer& archer, EnemyStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	EnemyState* HandleInput(const Action& action, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(EnemyStateRewindData* rewindData) override;


private:
	Archer& archer_;

	const float frameTime_{ 0.2f };
	float elapsedFrameTime_{ 0.0f };

	const float alertTime_{ 1.0f };
	float elapsedAlertTime_{ 0.0f };
};