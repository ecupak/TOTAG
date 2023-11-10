// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023
#include "precomp.h"
#include "Game.h"

#include "GameState.h"
#include "TitleScreenState.h"

#include "Shape.h"
#include "MapData.h"

#include "Camera.h"
#include "KeyboardManager.h"
#include "Renderer.h"

#include "ObstacleCollisionManager.h"
#include "TileCollisionManager.h"
#include "ObjectCollisionManager.h"

#include "Level.h"
#include "Subject.h"

#include "ParticleSystem.h"

#include "Scorekeeper.h"
#include "AudioManager.h"
#include "RewindManager.h"
#include "GameRewindData.h"
#include "SaveSystem.h"


// Player.
#include "Player.h"
#include "PlayerPlacer.h"

// Archers.
#include "Archer.h"
#include "ArcherList.h"
#include "ArcherConstructor.h"

// Knights.
#include "Knight.h"
#include "KnightList.h"
#include "KnightConstructor.h"

// Swinging ropes.
#include "SwingingRope.h"
#include "SwingingRopeList.h"
#include "SwingingRopeConstructor.h"

// Coins.
#include "Coin.h"
#include "CoinList.h"
#include "CoinConstructor.h"

// Souls.
#include "Soul.h"
#include "SoulList.h"
#include "SoulConstructor.h"

// Rock piles.
#include "RockPile.h"
#include "RockPileList.h"
#include "RockPileConstructor.h"

// Save points.
#include "SavePoint.h"
#include "SavePointList.h"
#include "SavePointConstructor.h"

// Pooled objects.
#include "RockPool.h"
#include "ArrowPool.h"

// Exit.
#include "LevelExit.h"
#include "LevelExitPlacer.h"


Game::Game()
	: keyboardManager_{ new KeyboardManager{} }
	, audioManager_{ new AudioManager{} }
	, camera_{ new Camera{} }
	, level_{ new Level{} }
	, scorekeeper_{ new Scorekeeper{} }
	, rewindManager_{ new RewindManager }
	, saveSystem_{ new SaveSystem{*rewindManager_} }
{	}


Game::~Game() 	
{ 
	delete level_;
	delete keyboardManager_;
	delete camera_;
	delete scorekeeper_;
	delete audioManager_;
	delete rewindManager_;
	delete saveSystem_;

	delete player_;
	delete playerPlacer_;
	
	delete archerList_;
	delete archerConstructor_;
	
	delete knightList_;
	delete knightConstructor_;
	
	delete ropeList_;
	delete ropeConstructor_;
	
	delete coinList_;
	delete coinConstructor_;

	delete soulList_;
	delete soulConstructor_;

	delete rockPileList_;
	delete rockPileConstructor_;
	
	delete savePointList_;
	delete savePointConstructor_;

	delete rockPool_;
	delete arrowPool_;
	
	delete levelExit_;
	delete levelExitPlacer_;

	delete renderer_;
	delete particleSystem_;
	delete obstacleCM_;
	delete tileCM_;
	delete objectCM_;
	delete activeList_;

	delete state_;
}


// -----------------------------------------------------------
// 
// Initialize the application.
// 
// -----------------------------------------------------------
void Game::Init()
{
	activeList_ = new GameObject* [100];

	InitCamera();
	InitCollisionManagers();
	InitRenderer();
	
	level_->onLevelStart_->AddObserver(audioManager_);

	// Initial game state.
	state_ = new TitleScreenState{ *this };
	state_->Enter();
}


// -----------------------------------------------------------
// 
// Input handling
// 
// -----------------------------------------------------------
void Game::KeyUp(int keycode)
{ 
	keyboardManager_->KeyReleased(keycode); 
}


void Game::KeyDown(int keycode)
{ 
	keyboardManager_->KeyPressed(keycode); 
}


// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// 
// There are 3 states that govern the execution of code:
// 1 Title Screen State
// 2 Play  Screen State
// 3 End   Screen State
//
// The code within Game can be viewed as building blocks for the states.
// 
// The states are all friends of Game and thus have full access to 
// everything here.
//  
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
	//if (keyboardManager_->IsPressed(Action::ZoomIn))
	//{
	//	camera_->ChangeZoomLevel(-zoomIncrement);
	//}
	//
	//if (keyboardManager_->IsPressed(Action::ZoomOut))
	//{
	//	camera_->ChangeZoomLevel(zoomIncrement);
	//}

	keyboardManager_->Update();
	
	GameState* state{ state_->HandleInput() };
	
	if (state != nullptr)
	{
		ChangeState(state);
	}

	state_->Update(deltaTime);
}


// Change to new state.
void Game::ChangeState(GameState* newState)
{
	state_->Exit();

	delete state_;
	state_ = newState;

	state_->Enter();
}


// -----------------------------------------------------------
// 
// Persistent object initializer methods.
// 
// -----------------------------------------------------------


void Game::InitCamera()
{
	camera_->Initialize(stagingSprite_.GetSurface());
}


void Game::InitCollisionManagers()
{
	obstacleCM_ = new ObstacleCollisionManager{ level_->GetTileSize(), level_->GetMapData(), activeList_ };

	tileCM_ = new TileCollisionManager{ level_->GetTileSize(), level_->GetMapData(), activeList_ };

	objectCM_ = new ObjectCollisionManager{ *camera_, activeList_ };
}


void Game::InitRenderer()
{
	renderer_ = new Renderer{ stagingSprite_.GetSurface(), screen, *camera_, activeList_, level_->GetTilesetSprite(), level_->GetLayerData() };
}


// -----------------------------------------------------------
// 
// Level creation methods.
// 
// -----------------------------------------------------------


void Game::CreateLevel(const int levelIndex)
{
	// Reset the object lists and pool. Reset the player and rewind manager.
	ResetLevel();

	// Create the new level.
	const char* filepath{ "" };

	switch (levelIndex)
	{
	case 0:
		filepath = "Tiled/Desert_Ruins.tmx";
		break;
	default:
		// Load title screen.
		break;
	}

	level_->LoadLevel(filepath);

	PlaceObjects();

	rewindManager_->CreateFrameData(60);
}


void Game::ResetLevel()
{
	// Delete all actors and props (including their lists, pools, and constructors).
	// Easier than trying to find all the different variables and set them back to default.
	DeleteProps();
	DeletePlayer();
	DeleteEnemies();
	
	// Recreate all actors and props.
	InitProps();	// First because rocks and arrows need to be made before player and archers.
	InitPlayer();
	InitEnemies();

	// Reset score.
	scorekeeper_->SetScore(Scorekeeper::Score::Other, 0);
	scorekeeper_->SetScore(Scorekeeper::Score::Soul, 0);

	// Reset rewind data.
	rewindManager_->Reset();
}


void Game::DeleteProps()
{
	// Arrows ...
	delete arrowPool_;
	arrowPool_ = nullptr;

	// Swinging rope...
	delete ropeList_;
	delete ropeConstructor_;
	ropeList_ = nullptr;
	ropeConstructor_ = nullptr;

	// Rocks ...
	delete rockPool_;
	rockPool_ = nullptr;

	// Rock piles...
	delete rockPileList_;
	delete rockPileConstructor_;
	rockPileList_ = nullptr;
	rockPileConstructor_ = nullptr;

	// Coins...
	delete coinList_;
	delete coinConstructor_;
	coinList_ = nullptr;
	coinConstructor_ = nullptr;

	// Souls...
	delete soulList_;
	delete soulConstructor_;
	soulList_ = nullptr;
	soulConstructor_ = nullptr;

	// Save points...
	delete savePointList_;
	delete savePointConstructor_;
	savePointList_ = nullptr;
	savePointConstructor_ = nullptr;

	// Exit...
	delete levelExit_;
	delete levelExitPlacer_;
	levelExitPlacer_ = nullptr;
	levelExitPlacer_ = nullptr;

	// Particles...
	delete particleSystem_;
	particleSystem_ = nullptr;
}


void Game::DeletePlayer()
{
	// Player...
	delete player_;
}


void Game::DeleteEnemies()
{
	// Archers...
	delete archerList_;
	delete archerConstructor_;
	archerList_ = nullptr;
	archerConstructor_ = nullptr;

	// Knights...
	delete knightList_;
	delete knightConstructor_;
	knightList_ = nullptr;
	knightConstructor_ = nullptr;
}


void Game::InitProps()
{
	// Arrows ...
	int maxArrows{ 20 };
	rewindManager_->SetRewindArraySize(ObjectClass::Arrow, maxArrows);
	arrowPool_ = new ArrowPool{ maxArrows };

	// Swinging rope...
	ropeList_ = new SwingingRopeList{};
	ropeConstructor_ = new SwingingRopeConstructor{};

	// Rocks ...
	int maxRocks{ 20 };
	rewindManager_->SetRewindArraySize(ObjectClass::Rock, maxRocks);
	rockPool_ = new RockPool{ maxRocks };

	// Rock piles...
	rockPileList_ = new RockPileList{};
	rockPileConstructor_ = new RockPileConstructor{};

	// Coins...
	coinList_ = new CoinList{};
	coinConstructor_ = new CoinConstructor{};

	// Souls...
	soulList_ = new SoulList{};
	soulConstructor_ = new SoulConstructor{};

	// Save points...
	savePointList_ = new SavePointList{};
	savePointConstructor_ = new SavePointConstructor{};

	// Exit...
	levelExit_ = new LevelExit{};

	// Particles...
	particleSystem_ = new ParticleSystem{};
}


void Game::InitPlayer()
{
	// Player...
	player_ = new Player{ rockPool_ };
	playerPlacer_ = new PlayerPlacer{};
}


void Game::InitEnemies()
{
	// Archers...
	archerList_ = new ArcherList{};
	archerConstructor_ = new ArcherConstructor{};

	// Knights...
	knightList_ = new KnightList{};
	knightConstructor_ = new KnightConstructor{};
}


void Game::PlaceObjects()
{
	// Player.
	playerPlacer_->Place(*player_, level_->GetObjectData());

	// Enemies.
	rewindManager_->SetRewindArraySize(
		ObjectClass::Archer,
		archerConstructor_->Construct(
			*archerList_, level_->GetObjectData(), scorekeeper_, &player_->shape_->center_, arrowPool_
		)
	);

	rewindManager_->SetRewindArraySize(
		ObjectClass::Knight,
		knightConstructor_->Construct(
			*knightList_, level_->GetObjectData(), scorekeeper_, &player_->shape_->center_
		)
	);

	// Ropes.
	rewindManager_->SetRewindArraySize(
		ObjectClass::Rope,
		ropeConstructor_->Construct(
			*ropeList_, level_->GetObjectData(), camera_
		)
	);

	// Coins.
	rewindManager_->SetRewindArraySize(
		ObjectClass::Coin,
		coinConstructor_->Construct(
			*coinList_, level_->GetObjectData(), scorekeeper_, audioManager_
		)
	);

	// Souls.
	rewindManager_->SetRewindArraySize(
		ObjectClass::Soul,
		soulConstructor_->Construct(
			*soulList_, level_->GetObjectData(), scorekeeper_, audioManager_
		)
	);

	// Rock piles.
	rewindManager_->SetRewindArraySize(
		ObjectClass::RockPile,
		rockPileConstructor_->Construct(
			*rockPileList_, level_->GetObjectData(), audioManager_
		)
	);

	// Save points.
	rewindManager_->SetRewindArraySize(
		ObjectClass::SavePoint,
		savePointConstructor_->Construct(
			*savePointList_, level_->GetObjectData(), saveSystem_, particleSystem_
		)
	);

	// Particles.
	rewindManager_->SetRewindArraySize(ObjectClass::Particles, 1);

	// Exit.
	levelExitPlacer_->Place(*levelExit_, level_->GetObjectData());

	// Camera.
	camera_->SetFocus(player_);
	camera_->SetLevelBounds(level_->GetTileSize());
}


// -----------------------------------------------------------
// 
// Game loop methods.
// 
// -----------------------------------------------------------


// Store starting frame data to rewind frame arrays.
void Game::StoreRewindData()
{
	// Store game data.
	rewindManager_->GetGameRewindData()->currentSeed_ = GetSeed();
	rewindManager_->GetGameRewindData()->currentScoreOther_ = scorekeeper_->GetScore(Scorekeeper::Score::Other);
	rewindManager_->GetGameRewindData()->currentScoreSouls_ = scorekeeper_->GetScore(Scorekeeper::Score::Soul);

	// Store object data.
	player_->StoreRewindData(rewindManager_->GetPlayerRewindData());
	archerList_->StoreRewindData(rewindManager_->GetArcherRewindData());
	knightList_->StoreRewindData(rewindManager_->GetKnightRewindData());

	ropeList_->StoreRewindData(rewindManager_->GetSwingingRopeRewindData());
	rockPileList_->StoreRewindData(rewindManager_->GetRockPileRewindData());
	savePointList_->StoreRewindData(rewindManager_->GetSavePointRewindData());
	coinList_->StoreRewindData(rewindManager_->GetCoinRewindData());
	soulList_->StoreRewindData(rewindManager_->GetSoulRewindData());

	arrowPool_->StoreRewindData(rewindManager_->GetArrowRewindData());
	rockPool_->StoreRewindData(rewindManager_->GetRockRewindData());

	// Store particle data.
	particleSystem_->StoreRewindData(*rewindManager_->GetParticleRewindData());

	// Store camera data.
	camera_->StoreRewindData(*rewindManager_->GetCameraRewindData());
}


void Game::ApplyRewindData(const bool isFromSaveFile)
{
	// Apply rewind data to game data.
	SetSeed(rewindManager_->GetGameRewindData()->currentSeed_);
	scorekeeper_->SetScore(Scorekeeper::Score::Other, rewindManager_->GetGameRewindData()->currentScoreOther_);
	if (isFromSaveFile)
	{
		scorekeeper_->SetScore(Scorekeeper::Score::Soul, rewindManager_->GetGameRewindData()->currentScoreSouls_);
	}

	// Apply rewind data to objects.
	player_->Rewind(rewindManager_->GetPlayerRewindData());

	archerList_->Rewind(rewindManager_->GetArcherRewindData());
	knightList_->Rewind(rewindManager_->GetKnightRewindData());

	ropeList_->Rewind(rewindManager_->GetSwingingRopeRewindData());
	rockPileList_->Rewind(rewindManager_->GetRockPileRewindData());
	savePointList_->Rewind(rewindManager_->GetSavePointRewindData());
	coinList_->Rewind(rewindManager_->GetCoinRewindData());
	soulList_->Rewind(rewindManager_->GetSoulRewindData(), isFromSaveFile);

	arrowPool_->Rewind(rewindManager_->GetArrowRewindData());
	rockPool_->Rewind(rewindManager_->GetRockRewindData());

	// Apply rewind data to particles.
	particleSystem_->Rewind(*rewindManager_->GetParticleRewindData());

	// Apply rewind data to camera
	camera_->Rewind(*rewindManager_->GetCameraRewindData());
}


void Game::ResumeFromRewind()
{
	player_->Resume(rewindManager_->GetPlayerRewindData());

	archerList_->Resume(rewindManager_->GetArcherRewindData());
	knightList_->Resume(rewindManager_->GetKnightRewindData());

	ropeList_->Resume(rewindManager_->GetSwingingRopeRewindData());
	rockPileList_->Resume(rewindManager_->GetRockPileRewindData());
	savePointList_->Resume(rewindManager_->GetSavePointRewindData());
	coinList_->Resume(rewindManager_->GetCoinRewindData());
	soulList_->Resume(rewindManager_->GetSoulRewindData());

	arrowPool_->Resume(rewindManager_->GetArrowRewindData());
	rockPool_->Resume(rewindManager_->GetRockRewindData());

	camera_->Resume(*rewindManager_->GetCameraRewindData());
}


void Game::Update(const float deltaTime)
{
	UpdateObjects(deltaTime);
	UpdateParticleSystems(deltaTime);
}


void Game::UpdateObjects(const float deltaTime)
{
	// Normal update calls with time going forward.
	player_->Update(deltaTime, gravity, *keyboardManager_);
	archerList_->Update(deltaTime, gravity);
	knightList_->Update(deltaTime, gravity);

	ropeList_->Update(deltaTime);

	arrowPool_->Update(deltaTime, gravity);
	rockPool_->Update(deltaTime, gravity);
}


void Game::UpdateParticleSystems(const float deltaTime)
{
	particleSystem_->Update(deltaTime);
}


void Game::CollectActiveObjects()
{
	// Collect all active gameobjects into list.
	activeListCurrentIndex_ = 0;

	// Props drawn behind actors.
	ropeList_->AddActive(activeList_, activeListCurrentIndex_);
	coinList_->AddActive(activeList_, activeListCurrentIndex_);
	soulList_->AddActive(activeList_, activeListCurrentIndex_);
	activeList_[activeListCurrentIndex_++] = levelExit_;

	// Actors (draw player on top of other).
	archerList_->AddActive(activeList_, activeListCurrentIndex_);
	knightList_->AddActive(activeList_, activeListCurrentIndex_);
	activeList_[activeListCurrentIndex_++] = player_;

	// Props drawn on top of actors.
	savePointList_->AddActive(activeList_, activeListCurrentIndex_);
	rockPileList_->AddActive(activeList_, activeListCurrentIndex_);
	rockPool_->AddActive(activeList_, activeListCurrentIndex_);
	arrowPool_->AddActive(activeList_, activeListCurrentIndex_);
}


void Game::ProcessCollisions()
{
	obstacleCM_->ProcessCollisions(activeListCurrentIndex_);
	tileCM_->ProcessCollisions(activeListCurrentIndex_);
	objectCM_->ProcessCollisions(activeListCurrentIndex_);
}


void Game::RenderGame(const float deltaTime, const bool isRewindStep)
{
	// Clear stage with sky color.
	stagingSprite_.GetSurface()->Clear(0xd4f0f6);

	// Camera moves to center on player while keeping in bounds.
	// Happens here and not in UpdateObjects because player may be moved by collisions.
	if (!isRewindStep)
	{
		camera_->Update(deltaTime);
	}

	// Draw view at 16x16 resolution onto stage.
	renderer_->RenderView(activeListCurrentIndex_);
	particleSystem_->Render(stagingSprite_.GetSurface(), static_cast<int>(camera_->shape_->start_.x) + camera_->margin_.x, static_cast<int>(camera_->shape_->start_.y) + camera_->margin_.y);

	// Draw scaled to main screen.	
	stagingSprite_.DrawSectionScaled(0, 0, static_cast<int>(camera_->shape_->size_.x - (2 * camera_->margin_.x)), static_cast<int>(camera_->shape_->size_.y - (2 * camera_->margin_.y)),
		0, 0, SCRWIDTH, SCRHEIGHT, screen
	);

	// Draw HUD over screen.
	renderer_->RenderHUD(scorekeeper_, player_);
}


void Game::RenderTitleScreen(const bool hasSaveFile)
{
	Surface& stage{ *stagingSprite_.GetSurface() };
	
	stage.Clear(0);
	stage.PrintCenter("= Pitfall =", (stage.height >> 1) - 6, 0xFFFFFF);
	stage.PrintCenter("The Temple of the Absent God", (stage.height >> 1) + 6, 0xFFFFFF);

	if (hasSaveFile)
	{
		stage.PrintCenter("Press 'J' to start a new game", stage.height - 25, 0xFFFF00);
		stage.PrintCenter("Press 'L' to load your saved game", stage.height - 15, 0xA0FFA0);
	}
	else
	{
		stage.PrintCenter("Press 'J' to start", stage.height - 20, 0xFFFF00);
	}


	// Draw scaled to main screen.	
	stagingSprite_.DrawSectionScaled(0, 0, stage.width, stage.height, 0, 0, SCRWIDTH, SCRHEIGHT, screen);
}


void Game::RenderGameoverScreen(const bool hasSaveFile)
{
	Surface& stage{ *stagingSprite_.GetSurface() };

	stage.Clear(0);
	stage.PrintCenter("Game Over", (stage.height >> 2), 0xA0A0A0);
	
	PrintScore(stage);

	stage.PrintCenter("You have rejoined the sands", (stage.height >> 1) + (stage.height >> 3), 0xA00000);
	stage.PrintCenter("You have rejoined the sands", (stage.height >> 1) + (stage.height >> 3) + 1, 0xA0A0A0, 1, 1);

	if (hasSaveFile)
	{
		stage.PrintCenter("Press 'J' to return to the title screen", stage.height - 25, 0xFFFF00);
		stage.PrintCenter("Press 'L' to load your last save point", stage.height - 15, 0xA0FFA0, 1, -3);
	}
	else
	{
		stage.PrintCenter("Press 'J' to return to the title screen", stage.height - 20, 0xFFFF00);
	}

	// Draw scaled to main screen.	
	stagingSprite_.DrawSectionScaled(0, 0, stage.width, stage.height, 0, 0, SCRWIDTH, SCRHEIGHT, screen);
}


const bool Game::FindLevelToLoad()
{
	levelIndex_ = saveSystem_->GetSavedLevelIndex();

	return (levelIndex_ > -1);
}


void Game::RenderVictoryScreen()
{
	Surface& stage{ *stagingSprite_.GetSurface() };

	stage.Clear(0);
	stage.PrintCenter("Victory", (stage.height >> 2), 0xFFFFFF);

	PrintScore(stage);
	
	stage.PrintCenter("The sands tremble with excitement", (stage.height >> 1) + (stage.height >> 3), 0x0000FF);
	stage.PrintCenter("The sands tremble with excitement", (stage.height >> 1) + (stage.height >> 3) + 1, 0xAAAA00, 1, 1);

	stage.PrintCenter("Press 'J' to return to the title screen", stage.height - 25, 0xFFFF00);
	stage.PrintCenter("Press 'K' to start a new game", stage.height - 15, 0xA0FFA0, 1, -3);

	// Draw scaled to main screen.	
	stagingSprite_.DrawSectionScaled(0, 0, stage.width, stage.height, 0, 0, SCRWIDTH, SCRHEIGHT, screen);
}


void Game::PrintScore(Surface& stage)
{
	// Combine score value with label.
	char buffer[10];
	ec::GetCharFromInt(scorekeeper_->GetScore(Scorekeeper::Score::Both), buffer);

	char score[40];
	strcpy(score, "Score: ");
	strcat(score, buffer);

	// Get separate score values.

	// ... Coins.
	ec::GetCharFromInt(scorekeeper_->GetScore(Scorekeeper::Score::Other), buffer);

	char otherScore[20];
	strcpy(otherScore, "   ");
	strcat(otherScore, buffer);
	
	// ... Soul coins.
	ec::GetCharFromInt(scorekeeper_->GetScore(Scorekeeper::Score::Soul), buffer);

	char soulScore[20];
	strcpy(soulScore, "   ");
	strcat(soulScore, buffer);

	// Print
	// Sorry about the magic numbers, this is just dirty UI layout stuff.
	stage.PrintCenter(score, (stage.height >> 2) + 10, 0xAAAAAA);

	coinConstructor_->GetSprite().Draw(&stage, 
		stage.WhatIfPrintCenter(otherScore, 1) - 30, 
		(stage.height >> 2) + 30 - 6
	);
	stage.PrintCenter(otherScore, (stage.height >> 2) + 30, 0xAAAA00, 1, -30);

	soulConstructor_->GetSprite().Draw(&stage,
		stage.WhatIfPrintCenter(soulScore, 1) + 30,
		(stage.height >> 2) + 30 - 6
	);
	stage.PrintCenter(soulScore, (stage.height >> 2) + 30, 0x00AAAA, 1, 30);
}