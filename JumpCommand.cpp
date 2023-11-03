// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#include "precomp.h"
#include "JumpCommand.h"

#include "Actor.h"
#include "Vector2.h"


JumpCommand::JumpCommand(const Actor* actor, const bool willApplyImpulse, const int xDirection)
	: Command{ actor, xDirection, 0 }
	, willApplyImpulse_{ willApplyImpulse }
{	}


void JumpCommand::Execute(const float deltaTime)
{
	// Horizontal movement.
	actor_->velocity_->x += actor_->horizontalAcceleration_ * direction_.x * deltaTime;

	// Vertical movement.
	if (willApplyImpulse_)
	{
		actor_->velocity_->y = actor_->jumpImpulse_;
	}
}