#pragma once

struct PlayerRewindData;
struct ArcherRewindData;
struct KnightRewindData;
struct SwingingRopeRewindData;
struct ArrowRewindData;
struct CoinRewindData;
struct SoulRewindData;
struct RockRewindData;
struct RockPileRewindData;
struct SavePointRewindData;
struct ParticleRewindData;
struct CameraRewindData;
struct GameRewindData;


class FrameData
{
public:
	FrameData() = default;
	FrameData(const FrameData&) = delete;
	FrameData& operator=(const FrameData&) = delete;
	FrameData(FrameData&&) = delete;
	FrameData& operator=(FrameData&&) = delete;
	~FrameData();


	void Initialize(const int archers, const int knights, const int ropes, 
		const int arrows, const int coins, const int souls, const int rocks, 
		const int rockPiles, const int savePointCount_, const int particleCount_);

	PlayerRewindData* playerData_{ nullptr };
	ArcherRewindData* archerData_{ nullptr };
	KnightRewindData* knightData_{ nullptr };
	SwingingRopeRewindData* ropeData_{ nullptr };
	ArrowRewindData* arrowData_{ nullptr };
	CoinRewindData* coinData_{ nullptr };
	SoulRewindData* soulData_{ nullptr };
	RockRewindData* rockData_{ nullptr };
	RockPileRewindData* rockPileData_{ nullptr };
	SavePointRewindData* savePointData_{ nullptr };
	ParticleRewindData* particleData_{ nullptr };
	CameraRewindData* cameraData_{ nullptr };
	GameRewindData* gameData_{ nullptr };

private:
	void ReleaseMemory();
};