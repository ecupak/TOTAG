#pragma once

class LevelExit : public GameObject
{
public:
	LevelExit();
	
	virtual ~LevelExit() = default;


	void Place(const float2 startTileToCenterOn);


	void Draw(Surface* screen, int x, int y) override;
};