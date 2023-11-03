// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#pragma once

#include "DirectionalCommand.h"


class BounceCommand : public DirectionalCommand
{
public:
	BounceCommand(const float2 bounceVelocity);

	void Execute(Player* player, float deltaTime) override;

private:
	const float2 bounceVelocity_{ 0.0f, 0.0f };
};

