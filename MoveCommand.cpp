// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.

#include "precomp.h"
#include "MoveCommand.h"

#include "Actor.h"
#include "Vector2.h"


MoveCommand::MoveCommand(const Actor* actor, const int xDirection)
	: Command{ actor, xDirection, 0 }
{	}


void MoveCommand::Execute(const float deltaTime)
{
	// If velocity is within max range, allow acceleration to be applied and then limit velocity to be within max range.
	// Only allowed outside of max range if set by the launch command.
	if (actor_->velocity_->x > -(actor_->velocityMax_.x) && actor_->velocity_->x < actor_->velocityMax_.x)
	{
		actor_->velocity_->x = clamp(actor_->velocity_->x + actor_->horizontalAcceleration_ * direction_.x * deltaTime, -(actor_->velocityMax_.x), actor_->velocityMax_.x);
	}
}