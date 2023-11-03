// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#pragma once


class LaunchCommand : public Command
{
public:
	LaunchCommand(const Actor* actor, const int xDirection, const float2 rightwardLaunchVelocity);

	LaunchCommand(const Actor* actor, const float2 launchVelocity);

	void Execute(const float deltaTime) override;

private:
	float2 launchVelocity_{ 0.0f, 0.0f };
	const bool hasDirection_{ false };
};

