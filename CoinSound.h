#pragma once

class CoinSound
{
public:
	CoinSound();

	
	void Play();


private:
	int currentSoundIndex_{ 0 };

	Audio::Sound coin1_{ "assets/sounds/coin1.wav" };
	Audio::Sound coin2_{ "assets/sounds/coin2.wav" };
	Audio::Sound coin3_{ "assets/sounds/coin3.wav" };
	Audio::Sound coin4_{ "assets/sounds/coin4.wav" };
};