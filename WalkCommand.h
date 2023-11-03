// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.

#pragma once

#include "MoveCommand.h"


class WalkCommand : public MoveCommand
{
public:
	WalkCommand(const int xDirection = 0);

	void Execute(Player* player, float deltaTime) override;
};

