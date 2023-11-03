// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.

#include "precomp.h"
#include "WalkCommand.h"

#include "Player.h"
#include "Vector2.h"


WalkCommand::WalkCommand(const int xDirection)
	: MoveCommand{ xDirection, 0 }
{	}


void WalkCommand::Execute(Player* player, float deltaTime)
{
	player->velocity_->x += player->groundAcceleration_.x * direction_.x * deltaTime;
}