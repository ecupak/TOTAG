#pragma once

class RewindManager;

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
struct GameRewindData;

struct PlayerStateRewindData;
struct EnemyRewindData;
struct EnemyStateRewindData;
struct ActorRewindData;
struct GameObjectRewindData;

// [Credit] https://github.com/nlohmann/json
using json = nlohmann::json;


class SaveSystem : public Observer
{
public:
	SaveSystem(RewindManager& rewindManager);

	void SaveData();

	const bool LoadData();

	const int GetSavedLevelIndex();

	void OnNotify(const ObjectClass objectClass, const Event event) override;


private:
	// Save methods.
	void SavePlayerData(json& data);
	void SavePlayerStateRewindData(PlayerStateRewindData& rewindData, json& saveData);

	void SaveArcherData(json& data);
	void SaveKnightData(json& data);
	void SaveEnemyStateData(EnemyStateRewindData& rewindData, json& saveData);
	void SaveEnemyData(EnemyRewindData& rewindData, json& saveData);

	void SaveRopeData(json& saveData);
	void SaveArrowData(json& saveData);
	void SaveCoinData(json& saveData);
	void SaveSoulData(json& saveData);
	void SaveRockData(json& saveData);
	void SaveRockPileData(json& saveData);
	void SaveSavePointData(json& saveData);
	void SaveParticleData(json& saveData);
	void SaveGameData(json& saveData);

	void SaveActorData(ActorRewindData& rewindData, json& saveData);
	void SaveGameObjectData(GameObjectRewindData& rewindData, json& saveData);

	// Load methods.
	void LoadPlayerData(json& data);
	void LoadPlayerStateRewindData(PlayerStateRewindData& rewindData, json& saveData);

	void LoadArcherData(json& data);
	void LoadKnightData(json& data);
	void LoadEnemyStateData(EnemyStateRewindData& rewindData, json& saveData);
	void LoadEnemyData(EnemyRewindData& rewindData, json& saveData);

	void LoadRopeData(json& saveData);
	void LoadArrowData(json& saveData);
	void LoadCoinData(json& saveData);
	void LoadSoulData(json& saveData);
	void LoadRockData(json& saveData);
	void LoadRockPileData(json& saveData);
	void LoadSavePointData(json& saveData);
	void LoadParticleData(json& saveData);
	void LoadGameData(json& saveData);

	void LoadActorData(ActorRewindData& rewindData, json& saveData);
	void LoadGameObjectData(GameObjectRewindData& rewindData, json& saveData);


	json saveData_;
	RewindManager& rewinder_;
};