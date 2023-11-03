#include "precomp.h"
#include "CoinSound.h"


CoinSound::CoinSound() {}


void CoinSound::Play()
{
	switch (currentSoundIndex_)
	{
	case 0:
		coin1_.replay();
		break;
	case 1:
		coin2_.replay();
		break;
	case 2:
		coin3_.replay();
		break;
	case 3:
		coin4_.replay();
		break;
	default:
		coin1_.replay();
		break;
	}

	currentSoundIndex_ = ++currentSoundIndex_ % 4;
}