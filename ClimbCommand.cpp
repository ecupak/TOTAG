// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.

#include "precomp.h"
#include "ClimbCommand.h"

#include "Player.h"
#include "TileCollision.h"
#include "Vector2.h"


ClimbCommand::ClimbCommand(Player& player, const int yDirection)
	: PlayerCommand{ player, 0, yDirection }
{	}


void ClimbCommand::Execute(const float deltaTime)
{
	// Climb up or down (or stay still if no input).
	player_.velocity_->y = direction_.y * player_.climbingSpeed_ * deltaTime;

	// If climbing up and head is not touching rope, player is climbing up to ground. Give them a boost so collision is detected after leaving rope.
	if (direction_.y < 0.0f && !(player_.IsHitboxTouchingInteractTile(Player::HitboxRole::Head, TileCollisionID::Ladder)))
	{
		player_.velocity_->y -= 0.5f;
	}
}