#pragma once

class FrameData;

enum class ObjectClass;

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


class RewindManager
{
public:
	RewindManager();
	~RewindManager();

	void SetRewindArraySize(ObjectClass objectClass, const int size);

	void CreateFrameData(const int fps);
	
	void Reset();

	// Get the rewind data/arrays.
	PlayerRewindData* GetPlayerRewindData();
	ArcherRewindData* GetArcherRewindData();
	KnightRewindData* GetKnightRewindData();
	SwingingRopeRewindData* GetSwingingRopeRewindData();
	ArrowRewindData* GetArrowRewindData();
	CoinRewindData* GetCoinRewindData();
	SoulRewindData* GetSoulRewindData();
	RockRewindData* GetRockRewindData();
	RockPileRewindData* GetRockPileRewindData();
	SavePointRewindData* GetSavePointRewindData();
	ParticleRewindData* GetParticleRewindData();
	CameraRewindData* GetCameraRewindData();
	GameRewindData* GetGameRewindData();

	void AdvanceHead();
	void ControlHead(const int direction);
	void SetForwardLockToHead();

	const int GetKnightCount() const;
	const int GetArcherCount() const;
	const int GetCoinCount() const;
	const int GetSoulCount() const;
	const int GetRockPileCount() const;
	const int GetRockCount() const;
	const int GetRopeCount() const;
	const int GetArrowCount() const;
	const int GetSavePointCount() const;
	const int GetParticleCount() const;

private:
	int knightCount_{ 0 };
	int archerCount_{ 0 };
	int coinCount_{ 0 };
	int soulCount_{ 0 };
	int rockPileCount_{ 0 };
	int rockCount_{ 0 };
	int arrowCount_{ 0 };
	int ropeCount_{ 0 };
	int savePointCount_{ 0 };
	int particleCount_{ 0 };

	FrameData* frameData_{ nullptr };

	int head_{ 0 };
	int tail_{ 0 };
	int frameDataSize_{ 0 };
	int forwardLock_{ 0 };
};