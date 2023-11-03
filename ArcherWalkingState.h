#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class ArcherWalkingState : public EnemyState
{
public:
	ArcherWalkingState(Archer& archer);
	ArcherWalkingState(Archer& archer, EnemyStateRewindData* rewindData);

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
};



