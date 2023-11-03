#pragma once


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class ArcherStandingState : public EnemyState
{
public:
	ArcherStandingState(Archer& archer);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	EnemyState* HandleInput(const Action& action, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(EnemyStateRewindData* rewindData) override;


private:
	Archer& archer_;
};