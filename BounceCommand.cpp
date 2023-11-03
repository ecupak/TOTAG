// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#include "precomp.h"
#include "BounceCommand.h"

#include "Player.h"
#include "Vector2.h"


BounceCommand::BounceCommand(const float2 bounceVelocity)
	: bounceVelocity_{ bounceVelocity }
{	}


void BounceCommand::Execute(Player* player, float)
{
	*player->velocity_ = bounceVelocity_;
}