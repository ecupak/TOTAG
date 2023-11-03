#include "precomp.h"
#include "LevelSound.h"


LevelSound::LevelSound() {}


void LevelSound::Play()
{
	bgm1_.setLooping(true);
	bgm1_.play();
}


void LevelSound::AdjustSound(const float volumePercent, const float pitch)
{
	bgm1_.setVolume(volumePercent);
	bgm1_.setPitch(pitch);
}


void LevelSound::Stop()
{
	bgm1_.stop();
}


void LevelSound::FadeOut()
{
	bgm1_.setFade(0.0f, 1000);
}