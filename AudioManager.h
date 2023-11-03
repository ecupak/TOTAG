#pragma once

class CoinSound;
class SoulSound;
class LevelSound;
class RockPileSound;

enum class Event;
enum class ObjectClass;


class AudioManager : public Observer
{
public:
	AudioManager();
	~AudioManager() override;

	void OnNotify(const ObjectClass objectClass, const Event event) override;

	void AdjustSound(const float volumePercent, const float pitch);

	void FadeOut();


private:
	void PlayOnTouchAudio(const ObjectClass objectClass);
	
	
	void PlayOnDeathAudio(const ObjectClass objectClass);
	

	void PlayOnLevelStartAudio();

		
	CoinSound* coinSound_{ nullptr };
	SoulSound* soulSound_{ nullptr };
	LevelSound* levelSound_{ nullptr };
	RockPileSound* rockPileSound_{ nullptr };
};