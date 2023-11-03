// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#pragma once


class JumpCommand : public Command
{
public:
	JumpCommand(const Actor* actor, const bool willAddHeight, const int xDirection = 0);

	void Execute(const float deltaTime) override;

private:
	bool willApplyImpulse_{ false };
};

