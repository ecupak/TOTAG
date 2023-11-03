#pragma once


// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
class ThrowRockCommand : public PlayerCommand
{
public:
	ThrowRockCommand(Player& player);

	void Execute(const float deltaTime) override;
};


