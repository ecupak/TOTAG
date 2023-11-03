#pragma once

class Player;


// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
class PlayerCommand : public Command
{
protected:
	PlayerCommand(Player& player)
		: Command{}
		, player_{player}
	{	}

	PlayerCommand(Player& player, const int xDirection, const int yDirection)
		: Command{ xDirection, yDirection }
		, player_{ player }
	{	}

	Player& player_;
};