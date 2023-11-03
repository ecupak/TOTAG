#include "precomp.h"
#include "PlayerState.h"


#include "Player.h"

#include "KeyboardManager.h"


PlayerState::PlayerState(Player& player)
	: player_{ player }
{	}


const int PlayerState::GetOpposingDirectionsAsValue(const bool negativeDirection, const bool positiveDirection)
{
	return (negativeDirection ? -1 : 0) + (positiveDirection ? 1 : 0);
}