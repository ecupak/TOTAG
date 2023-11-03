#pragma once


// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
class ClimbCommand : public PlayerCommand
{
public:
	ClimbCommand(Player& player, const int yDirection = 0);

	void Execute(const float deltaTime) override;
};

