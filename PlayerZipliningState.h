#pragma once

struct MapTile;


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
class PlayerZipliningState : public PlayerState
{
public:
	enum Direction
	{
		Left = 0,
		Right,
	};


	PlayerZipliningState(Player& player, const MapTile& mapTile);
	PlayerZipliningState(Player& player, PlayerStateRewindData* rewindData);


	void SetHitboxData() override;


	void Enter(const bool isRewindEntry = false) override;


	PlayerState* HandleInput(const KeyboardManager& km, Command*& command) override;


	void Update(const float deltaTime) override;


	void Exit() override;


	void StoreRewindData(PlayerStateRewindData* rewindData) override;


	void SetPositionUnderZipline();


	int2 FindTileHitboxEnd();


private:
	const int directionOfDescent_{ Direction::Right };
};