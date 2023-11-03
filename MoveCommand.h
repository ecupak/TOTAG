#pragma once


// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
class MoveCommand : public Command
{
public:
	MoveCommand(const Actor* actor, const int xDirection = 0);

	void Execute(const float deltaTime) override;
};

