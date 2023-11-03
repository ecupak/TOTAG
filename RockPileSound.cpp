#include "precomp.h"
#include "RockPileSound.h"


RockPileSound::RockPileSound() {}


void RockPileSound::Play()
{
	pickupSound_.play();
}