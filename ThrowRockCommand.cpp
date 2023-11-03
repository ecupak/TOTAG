// [Credit] https://gameprogrammingpatterns.com/command.html
// Insiration for all command-based behavior here, in PlayerCommand, and in the various PlayerCommand-inheriting classes.

#include "precomp.h"
#include "ThrowRockCommand.h"

#include "Player.h"
#include "Shape.h"
#include "RockPool.h"

#include "TileCollision.h"
#include "Vector2.h"


ThrowRockCommand::ThrowRockCommand(Player& player)
	: PlayerCommand{ player }
{	}


void ThrowRockCommand::Execute(const float)
{
	if (player_.rockInventory_ <= 0)
	{
		return;
	}

	float2 velocity{
		200.0f * (player_.isFlipped_ ? -1 : 1),
		-50.0f
	};

	if (player_.rockPool_->Create(static_cast<float2>(player_.shape_->center_), velocity))
	{
		--player_.rockInventory_;
	}
}