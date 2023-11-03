#pragma once

class RockPileSound
{
public:
	RockPileSound();


	void Play();


private:
	Audio::Sound pickupSound_{ "assets/sounds/rockpile.wav" };
};
