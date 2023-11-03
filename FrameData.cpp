#include "precomp.h"
#include "FrameData.h"

#include "PlayerRewindData.h"
#include "ArcherRewindData.h"
#include "KnightRewindData.h"
#include "SwingingRopeRewindData.h"
#include "ArrowRewindData.h"
#include "CoinRewindData.h"
#include "SoulRewindData.h"
#include "RockRewindData.h"
#include "RockPileRewindData.h"
#include "SavePointRewindData.h"
#include "ParticleRewindData.h"
#include "CameraRewindData.h"
#include "GameRewindData.h"


FrameData::~FrameData()
{
	ReleaseMemory();
}


void FrameData::Initialize(const int archers, const int knights,
	const int ropes, const int arrows, const int coins, const int souls, const int rocks, 
	const int rockPiles, const int savePoints, const int particles)
{
	ReleaseMemory();

	playerData_ = new PlayerRewindData[1];	// To stay consistent with the rest.
	archerData_ = new ArcherRewindData[archers];
	knightData_ = new KnightRewindData[knights];
	ropeData_ = new SwingingRopeRewindData[ropes];
	arrowData_ = new ArrowRewindData[arrows];
	coinData_ = new CoinRewindData[coins];
	soulData_ = new SoulRewindData[souls];
	rockData_ = new RockRewindData[rocks];
	rockPileData_ = new RockPileRewindData[rockPiles];
	savePointData_ = new SavePointRewindData[savePoints];
	particleData_ = new ParticleRewindData[particles];
	cameraData_ = new CameraRewindData[1];
	gameData_ = new GameRewindData[1];
}


void FrameData::ReleaseMemory()
{
	delete[] playerData_;
	playerData_ = nullptr;

	delete[] archerData_;
	archerData_ = nullptr;

	delete[] knightData_;
	knightData_ = nullptr;

	delete[] ropeData_;
	ropeData_ = nullptr;

	delete[] arrowData_;
	arrowData_ = nullptr;

	delete[] coinData_;
	coinData_ = nullptr;

	delete[] soulData_;
	soulData_ = nullptr;

	delete[] rockData_;
	rockData_ = nullptr;

	delete[] rockPileData_;
	rockPileData_ = nullptr;

	delete[] savePointData_;
	savePointData_ = nullptr;

	delete[] particleData_;
	particleData_ = nullptr;

	delete[] cameraData_;
	cameraData_ = nullptr;

	delete[] gameData_;
	gameData_ = nullptr;
}