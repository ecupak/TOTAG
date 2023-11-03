#include "precomp.h"
#include "AudioManager.h"

#include "GameObject.h"
#include "CoinSound.h"
#include "RockPileSound.h"
#include "LevelSound.h"
#include "SoulSound.h"

#include "Event.h"


AudioManager::AudioManager()
	: coinSound_{ new CoinSound{} }
	, soulSound_{ new SoulSound{} }
	, levelSound_{ new LevelSound{} }
	, rockPileSound_{ new RockPileSound }
{	}


AudioManager::~AudioManager()
{
	delete coinSound_;
	delete soulSound_;
	delete levelSound_;
	delete rockPileSound_;
}


void AudioManager::OnNotify(const ObjectClass objectClass, const Event event)
{
	switch (event)
	{
	case Event::OnTouch:
		PlayOnTouchAudio(objectClass);
		break;
	case Event::OnDeath:
		PlayOnDeathAudio(objectClass);
		break;
	case Event::OnLevelStart:
		PlayOnLevelStartAudio();
		break;
	}
}


void AudioManager::PlayOnTouchAudio(const ObjectClass objectClass)
{
	switch (objectClass)
	{
	case ObjectClass::Coin:
		coinSound_->Play();
		break;
	case ObjectClass::Soul:
		soulSound_->Play();
		break;
	case ObjectClass::RockPile:
		rockPileSound_->Play();
		break;
	default:
		break;
	}
}


void AudioManager::PlayOnDeathAudio(const ObjectClass)
{

}


void AudioManager::PlayOnLevelStartAudio()
{
	levelSound_->Play();
}


void AudioManager::AdjustSound(const float volumePercent, const float pitch)
{
	levelSound_->AdjustSound(volumePercent, pitch);
}


void AudioManager::FadeOut()
{
	levelSound_->FadeOut();
}