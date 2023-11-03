// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.
#include "precomp.h"
#include "LaunchCommand.h"

#include "Actor.h"
#include "Vector2.h" // type of actor velocity.


LaunchCommand::LaunchCommand(const Actor* actor, const int xDirection, const float2 rightwardLaunchVelocity)
	: Command{ actor, xDirection, 0}
	, launchVelocity_{ rightwardLaunchVelocity }
	, hasDirection_{ true }
{	}


LaunchCommand::LaunchCommand(const Actor* actor, const float2 launchVelocity)
	: Command{ actor }
	, launchVelocity_{ launchVelocity }
	, hasDirection_{ false }
{	}


void LaunchCommand::Execute(const float)
{
	if (hasDirection_)
	{
		launchVelocity_.x *= direction_.x;
	}

	*actor_->velocity_ = launchVelocity_;
}