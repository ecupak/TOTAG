#include "precomp.h"
#include "SaveSystem.h"

#include "RewindManager.h"

#include "PlayerRewindData.h"
#include "PlayerStateRewindData.h"

#include "ActorRewindData.h"
#include "GameObjectRewindData.h"

#include "ArcherRewindData.h"
#include "KnightRewindData.h"
#include "EnemyStateRewindData.h"
#include "EnemyRewindData.h"

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

#include "Event.h"

// [Credit] https://github.com/nlohmann/json
using json = nlohmann::json;


SaveSystem::SaveSystem(RewindManager& rewindManager)
	: rewinder_{ rewindManager }
{	}


void SaveSystem::SaveData()
{
	// Save the data.
	json data;

	SavePlayerData(data["player"]);
	SaveArcherData(data["archers"]);
	SaveKnightData(data["knights"]);
	SaveRopeData(data["ropes"]);
	SaveArrowData(data["arrows"]);
	SaveCoinData(data["coins"]);
	SaveSoulData(data["souls"]);
	SaveRockData(data["rocks"]);
	SaveRockPileData(data["rockPiles"]);
	SaveSavePointData(data["savePoints"]);
	SaveParticleData(data["particles"]);
	SaveCameraData(data["camera"]);

	SaveGameData(data["game"]);

	// Output save file.
	ofstream saveFile("assets/save_game.json");
	saveFile << data.dump();
	saveFile.close();
}



const bool SaveSystem::LoadData()
{
	// Load data from file into first rewind frame.
	// Game will apply data as if rewinding (Rewind() + Resume()) at start.
	LoadPlayerData(saveData_["player"]);
	LoadArcherData(saveData_["archers"]);
	LoadKnightData(saveData_["knights"]);
	LoadRopeData(saveData_["ropes"]);
	LoadArrowData(saveData_["arrows"]);
	LoadCoinData(saveData_["coins"]);
	LoadSoulData(saveData_["souls"]);
	LoadRockData(saveData_["rocks"]);
	LoadRockPileData(saveData_["rockPiles"]);
	LoadSavePointData(saveData_["savePoints"]);
	LoadParticleData(saveData_["particles"]);
	LoadCameraData(saveData_["camera"]);

	LoadGameData(saveData_["game"]);

	return true;
}


const int SaveSystem::GetSavedLevelIndex()
{
	// Try to load file.
	ifstream saveFile("assets/save_game.json");

	if (!saveFile)
	{
		return -1;
	}

	saveData_.clear();
	saveData_ = json::parse(saveFile);

	return saveData_["game"]["levelIndex"];
}


void SaveSystem::OnNotify(const ObjectClass objectClass, const Event)
{
	if (objectClass == ObjectClass::SavePoint)
	{
		SaveData();
	}
}


// SAVE METHODS //


void SaveSystem::SavePlayerData(json& saveData)
{
	PlayerRewindData& rewindData{ *rewinder_.GetPlayerRewindData() };

	saveData["rockInventory"] = rewindData.rockInventory_;
	saveData["stateIndex"] = rewindData.stateIndex_;

	saveData["ladderDelay"] = rewindData.ladderDelay_;
	saveData["ziplineDelay"] = rewindData.ziplineDelay_;
	saveData["ropeDelay"] = rewindData.ropeDelay_;
	saveData["throwDelay"] = rewindData.throwDelay_;
	saveData["elapsedInvulnerableTime"] = rewindData.elapsedInvulnerableTime_;

	saveData["canGrabLadder"] = rewindData.canGrabLadder_;
	saveData["canGrabZipline"] = rewindData.canGrabZipline_;
	saveData["canGrabRope"] = rewindData.canGrabRope_;
	saveData["canThrow"] = rewindData.canThrow_;
	saveData["isInvulnerable"] = rewindData.isInvulnerable_;

	SavePlayerStateRewindData(*rewindData.playerStateRewindData_, saveData);
	SaveActorData(*rewindData.actorRewindData_, saveData);
}


void SaveSystem::SavePlayerStateRewindData(PlayerStateRewindData& rewindData, json& saveData)
{
	saveData["state"]["float2"]["x"] = rewindData.vFloat2_.x;
	saveData["state"]["float2"]["y"] = rewindData.vFloat2_.y;
	saveData["state"]["floatA"] = rewindData.vFloatA_;
	saveData["state"]["floatB"] = rewindData.vFloatB_;
	saveData["state"]["int"] = rewindData.vInt_;
	saveData["state"]["bool"] = rewindData.vBool_;
	// Don't save swinging rockPile pointer. Cannot save while on rockPile.
}


void SaveSystem::SaveArcherData(json& saveData)
{	
	for (int index{ 0 }; index < rewinder_.GetArcherCount(); ++index)
	{
		json archerData;
		archerData["archer"];

		ArcherRewindData& rewindData{ rewinder_.GetArcherRewindData()[index] };

		archerData["attackDelay"] = rewindData.attackDelay_;
		archerData["lockonSpriteFrame"] = rewindData.lockonSpriteFrame_;
		archerData["stateIndex"] = rewindData.stateIndex_;

		SaveEnemyStateData(*rewindData.enemyStateRewindData_, archerData);
		SaveEnemyData(*rewindData.enemyRewindData_, archerData);

		// Add to array.
		saveData.push_back(archerData);
	}
}


void SaveSystem::SaveKnightData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetKnightCount(); ++index)
	{
		json knightData;
		knightData["knight"];

		KnightRewindData& rewindData{ rewinder_.GetKnightRewindData()[index] };

		knightData["elapsedInvulnerableTime"] = rewindData.elapsedInvulnerableTime_;
		knightData["stateIndex"] = rewindData.stateIndex_;
		knightData["isInvulnerable"] = rewindData.isInvulnerable_;

		SaveEnemyStateData(*rewindData.enemyStateRewindData_, knightData);
		SaveEnemyData(*rewindData.enemyRewindData_, knightData);

		// Add to array.
		saveData.push_back(knightData);
	}
}


void SaveSystem::SaveRopeData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRopeCount(); ++index)
	{
		json swingingRopeData;
		swingingRopeData["swingingRope"];

		SwingingRopeRewindData& rewindData{ rewinder_.GetSwingingRopeRewindData()[index] };

		swingingRopeData["time"] = rewindData.time_;
		swingingRopeData["direction"] = rewindData.direction_;
		swingingRopeData["hasPassenger"] = rewindData.hasPassenger_;

		SaveGameObjectData(*rewindData.gameObjectRewindData_, swingingRopeData);
		
		// Add to array.
		saveData.push_back(swingingRopeData);
	}
}


void SaveSystem::SaveArrowData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetArrowCount(); ++index)
	{
		json arrowData;
		arrowData["arrow"];

		ArrowRewindData& rewindData{ rewinder_.GetArrowRewindData()[index] };

		arrowData["previousPosition"]["x"] = rewindData.previousPosition_.x;
		arrowData["previousPosition"]["y"] = rewindData.previousPosition_.y;
		arrowData["isStuck"] = rewindData.isStuck_;
		arrowData["lifetime"] = rewindData.lifetime_;

		SaveGameObjectData(*rewindData.gameObjectRewindData_, arrowData);

		// Add to array.
		saveData.push_back(arrowData);
	}
}


void SaveSystem::SaveCoinData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetCoinCount(); ++index)
	{
		json coinData;
		coinData["coin"];

		CoinRewindData& rewindData{ rewinder_.GetCoinRewindData()[index] };

		SaveGameObjectData(*rewindData.gameObjectRewindData_, coinData);

		// Add to array.
		saveData.push_back(coinData);
	}
}


void SaveSystem::SaveSoulData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetSoulCount(); ++index)
	{
		json soulData;
		soulData["soul"];

		SoulRewindData& rewindData{ rewinder_.GetSoulRewindData()[index] };

		SaveGameObjectData(*rewindData.gameObjectRewindData_, soulData);

		// Add to array.
		saveData.push_back(soulData);
	}
}


void SaveSystem::SaveRockData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRockCount(); ++index)
	{
		json rockData;
		rockData["rock"];

		RockRewindData& rewindData{ rewinder_.GetRockRewindData()[index] };

		rockData["lifetime"] = rewindData.lifetime_;;

		SaveGameObjectData(*rewindData.gameObjectRewindData_, rockData);

		// Add to array.
		saveData.push_back(rockData);
	}
}


void SaveSystem::SaveRockPileData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRockPileCount(); ++index)
	{
		json rockPileData;
		rockPileData["rockPile"];

		RockPileRewindData& rewindData{ rewinder_.GetRockPileRewindData()[index] };

		SaveGameObjectData(*rewindData.gameObjectRewindData_, rockPileData);

		// Add to array.
		saveData.push_back(rockPileData);
	}
}


void SaveSystem::SaveSavePointData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetSavePointCount(); ++index)
	{
		json savePointData;
		savePointData["savePoint"];

		SavePointRewindData& rewindData{ rewinder_.GetSavePointRewindData()[index] };

		SaveGameObjectData(*rewindData.gameObjectRewindData_, savePointData);

		// Add to array.
		saveData.push_back(savePointData);
	}
}


void SaveSystem::SaveParticleData(json& saveData)
{
	saveData["color"] = rewinder_.GetParticleRewindData()->color_;
}


void SaveSystem::SaveCameraData(json& saveData)
{
	saveData["currentZoom"] = rewinder_.GetCameraRewindData()->zoomLevel_;
	saveData["startZoom"] = rewinder_.GetCameraRewindData()->startZoomLevel_;
	saveData["finalZoom"] = rewinder_.GetCameraRewindData()->finalZoomLevel_;
	saveData["elapsedZoomTime"] = rewinder_.GetCameraRewindData()->elapsedZoomTime_;
}


void SaveSystem::SaveGameData(json& saveData)
{
	GameRewindData& rewindData{ *rewinder_.GetGameRewindData() };

	saveData["seed"] = rewindData.currentSeed_;
	saveData["levelIndex"] = rewindData.currentLevelIndex_;
	saveData["scoreOther"] = rewindData.currentScoreOther_;
	saveData["scoreSouls"] = rewindData.currentScoreSouls_;
}


void SaveSystem::SaveEnemyStateData(EnemyStateRewindData& rewindData, json& saveData)
{
	saveData["state"]["floatA"] = rewindData.vFloatA_;
	saveData["state"]["floatB"] = rewindData.vFloatB_;
	saveData["state"]["bool"] = rewindData.vBool_;
}


void SaveSystem::SaveEnemyData(EnemyRewindData& rewindData, json& saveData)
{
	saveData["enemy"]["currentAction"] = rewindData.currentAction_;
	saveData["enemy"]["stateDuration"] = rewindData.stateDuration_;

	SaveActorData(*rewindData.actorRewindData_, saveData);
}


void SaveSystem::SaveActorData(ActorRewindData& rewindData, json& saveData)
{
	saveData["actor"]["hp"] = rewindData.currentHp_;
	saveData["actor"]["ground"] = rewindData.isOnGround_;
	saveData["actor"]["wall"] = rewindData.isAgainstWall_;

	SaveGameObjectData(*rewindData.gameObjectRewindData_, saveData);
}


void SaveSystem::SaveGameObjectData(GameObjectRewindData& rewindData, json& saveData)
{
	saveData["object"]["position"]["x"] = rewindData.position_.x;
	saveData["object"]["position"]["y"] = rewindData.position_.y;
	saveData["object"]["velocity"]["x"] = rewindData.velocity_.x;
	saveData["object"]["velocity"]["y"] = rewindData.velocity_.y;

	saveData["object"]["spriteFrame"] = rewindData.spriteFrame_;
	saveData["object"]["overlaps"] = rewindData.overlappingTileLayers_;

	saveData["object"]["isFlipped"] = rewindData.isFlipped_;
	saveData["object"]["isOnstage"] = rewindData.isOnstage_;
	saveData["object"]["isActive"] = rewindData.isActive_;
}


// LOAD METHODS //


void SaveSystem::LoadPlayerData(json& saveData)
{
	PlayerRewindData& rewindData{ *rewinder_.GetPlayerRewindData() };

	rewindData.rockInventory_ = saveData["rockInventory"];
	rewindData.stateIndex_ = saveData["stateIndex"];

	rewindData.ladderDelay_ = saveData["ladderDelay"];
	rewindData.ziplineDelay_ = saveData["ziplineDelay"];
	rewindData.ropeDelay_ = saveData["ropeDelay"];
	rewindData.throwDelay_ = saveData["throwDelay"];
	rewindData.elapsedInvulnerableTime_ = saveData["elapsedInvulnerableTime"];

	rewindData.canGrabLadder_ = saveData["canGrabLadder"];
	rewindData.canGrabZipline_ = saveData["canGrabZipline"];
	rewindData.canGrabRope_ = saveData["canGrabRope"];
	rewindData.canThrow_ = saveData["canThrow"];
	rewindData.isInvulnerable_ = saveData["isInvulnerable"];

	LoadPlayerStateRewindData(*rewindData.playerStateRewindData_, saveData);
	LoadActorData(*rewindData.actorRewindData_, saveData);
}


void SaveSystem::LoadPlayerStateRewindData(PlayerStateRewindData& rewindData, json& saveData)
{
	rewindData.vFloat2_.x = saveData["state"]["float2"]["x"];
	rewindData.vFloat2_.y = saveData["state"]["float2"]["y"];
	rewindData.vFloatA_ = saveData["state"]["floatA"];
	rewindData.vFloatB_ = saveData["state"]["floatB"];
	rewindData.vInt_ = saveData["state"]["int"];
	rewindData.vBool_ = saveData["state"]["bool"];
	rewindData.vRopePtr_ = nullptr;
}


void SaveSystem::LoadArcherData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetArcherCount(); ++index)
	{
		json archerData{ saveData[index] };
		ArcherRewindData& rewindData{ rewinder_.GetArcherRewindData()[index] };

		rewindData.attackDelay_ = archerData["attackDelay"];
		rewindData.lockonSpriteFrame_ = archerData["lockonSpriteFrame"];
		rewindData.stateIndex_ = archerData["stateIndex"];

		LoadEnemyStateData(*rewindData.enemyStateRewindData_, archerData);
		LoadEnemyData(*rewindData.enemyRewindData_, archerData);
	}
}


void SaveSystem::LoadKnightData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetKnightCount(); ++index)
	{
		json knightData{ saveData[index] };
		KnightRewindData& rewindData{ rewinder_.GetKnightRewindData()[index] };

		rewindData.elapsedInvulnerableTime_ = knightData["elapsedInvulnerableTime"]; 
		rewindData.stateIndex_ = knightData["stateIndex"]; 
		rewindData.isInvulnerable_ = knightData["isInvulnerable"]; 

		LoadEnemyStateData(*rewindData.enemyStateRewindData_, knightData);
		LoadEnemyData(*rewindData.enemyRewindData_, knightData);
	}
}


void SaveSystem::LoadRopeData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRopeCount(); ++index)
	{
		json ropeData{saveData[index]};
		SwingingRopeRewindData& rewindData{ rewinder_.GetSwingingRopeRewindData()[index] };

		rewindData.time_ = ropeData["time"];
		rewindData.direction_ = ropeData["direction"];
		rewindData.hasPassenger_ = ropeData["hasPassenger"];

		LoadGameObjectData(*rewindData.gameObjectRewindData_, ropeData);
	}
}


void SaveSystem::LoadArrowData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetArrowCount(); ++index)
	{
		json arrowData{saveData[index]};
		ArrowRewindData& rewindData{ rewinder_.GetArrowRewindData()[index] };

		rewindData.previousPosition_.x = arrowData["previousPosition"]["x"];
		rewindData.previousPosition_.y = arrowData["previousPosition"]["y"];
		rewindData.isStuck_ = arrowData["isStuck"];
		rewindData.lifetime_ = arrowData["lifetime"];

		LoadGameObjectData(*rewindData.gameObjectRewindData_, arrowData);
	}
}


void SaveSystem::LoadCoinData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetCoinCount(); ++index)
	{
		json coinData{ saveData[index] };
		CoinRewindData& rewindData{ rewinder_.GetCoinRewindData()[index] };

		LoadGameObjectData(*rewindData.gameObjectRewindData_, coinData);
	}
}


void SaveSystem::LoadSoulData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetSoulCount(); ++index)
	{
		json soulData{ saveData[index] };
		SoulRewindData& rewindData{ rewinder_.GetSoulRewindData()[index] };

		LoadGameObjectData(*rewindData.gameObjectRewindData_, soulData);
	}
}


void SaveSystem::LoadRockData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRockCount(); ++index)
	{
		json rockData{ saveData[index] };
		RockRewindData& rewindData{ rewinder_.GetRockRewindData()[index] };

		rewindData.lifetime_ = rockData["lifetime"];

		LoadGameObjectData(*rewindData.gameObjectRewindData_, rockData);
	}
}


void SaveSystem::LoadRockPileData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetRockPileCount(); ++index)
	{
		json rockPileData{ saveData[index] };
		RockPileRewindData& rewindData{ rewinder_.GetRockPileRewindData()[index] };

		LoadGameObjectData(*rewindData.gameObjectRewindData_, rockPileData);
	}
}


void SaveSystem::LoadSavePointData(json& saveData)
{
	for (int index{ 0 }; index < rewinder_.GetSavePointCount(); ++index)
	{
		json savePointData{ saveData[index] };
		SavePointRewindData& rewindData{ rewinder_.GetSavePointRewindData()[index] };

		LoadGameObjectData(*rewindData.gameObjectRewindData_, savePointData);
	}
}


void SaveSystem::LoadParticleData(json& saveData)
{
	rewinder_.GetParticleRewindData()->color_ = saveData["color"];
}


void SaveSystem::LoadCameraData(json& saveData)
{
	rewinder_.GetCameraRewindData()->zoomLevel_ = saveData["currentZoom"];
	rewinder_.GetCameraRewindData()->startZoomLevel_ = saveData["startZoom"];
	rewinder_.GetCameraRewindData()->finalZoomLevel_ = saveData["finalZoom"];
	rewinder_.GetCameraRewindData()->elapsedZoomTime_ = saveData["elapsedZoomTime"];
}


void SaveSystem::LoadGameData(json& saveData)
{
	GameRewindData& rewindData{ *rewinder_.GetGameRewindData() };

	rewindData.currentSeed_ = saveData["seed"];
	rewindData.currentLevelIndex_ = saveData["levelIndex"];
	rewindData.currentScoreOther_ = saveData["scoreOther"];
	rewindData.currentScoreSouls_ = saveData["scoreSouls"];
}


void SaveSystem::LoadEnemyStateData(EnemyStateRewindData& rewindData, json& saveData)
{
	rewindData.vFloatA_ = saveData["state"]["floatA"];
	rewindData.vFloatB_ = saveData["state"]["floatB"];
	rewindData.vBool_ = saveData["state"]["bool"];
}


void SaveSystem::LoadEnemyData(EnemyRewindData& rewindData, json& saveData)
{
	rewindData.currentAction_ = saveData["enemy"]["currentAction"];
	rewindData.stateDuration_ = saveData["enemy"]["stateDuration"];

	LoadActorData(*rewindData.actorRewindData_, saveData);
}


void SaveSystem::LoadActorData(ActorRewindData& rewindData, json& saveData)
{
	rewindData.currentHp_ = saveData["actor"]["hp"];
	rewindData.isOnGround_ = saveData["actor"]["ground"];
	rewindData.isAgainstWall_ = saveData["actor"]["wall"];

	LoadGameObjectData(*rewindData.gameObjectRewindData_, saveData);
}


void SaveSystem::LoadGameObjectData(GameObjectRewindData& rewindData, json& saveData)
{
	rewindData.position_.x = saveData["object"]["position"]["x"];
	rewindData.position_.y = saveData["object"]["position"]["y"];
	rewindData.velocity_.x = saveData["object"]["velocity"]["x"];
	rewindData.velocity_.y = saveData["object"]["velocity"]["y"];

	rewindData.spriteFrame_ = saveData["object"]["spriteFrame"];
	rewindData.overlappingTileLayers_ = saveData["object"]["overlaps"];

	rewindData.isFlipped_ = saveData["object"]["isFlipped"];
	rewindData.isOnstage_ = saveData["object"]["isOnstage"];
	rewindData.isActive_ = saveData["object"]["isActive"];
}