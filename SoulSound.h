#pragma once

class SoulSound
{
public:
	SoulSound();

	
	void Play();


private:
	Audio::Sound soul_{ "assets/sounds/soul.wav" };
};