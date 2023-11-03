// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023
#pragma once

// States handle the actual code flow and are friends of this class.
class GameState;

// Input.
class KeyboardManager;

// The map and tiles everything dances on.
class Level;

// Visual team.
class Camera;
class Renderer;

// Beautifier.
class ParticleSystem;

// Collision managers.
class ObstacleCollisionManager;
class TileCollisionManager;
class ObjectCollisionManager;

// Support classes for score, sounds, rewind mechanic, and save/load.
class Scorekeeper;
class AudioManager;
class RewindManager;
class SaveSystem;

// Base class of all objects.
class GameObject;

// Player.
class Player;
class PlayerPlacer;

// Archers.
class ArcherList;
class ArcherConstructor;

// Knights.
class KnightList;
class KnightConstructor;

// Swinging ropes.
class SwingingRopeList;
class SwingingRopeConstructor;

// Coins.
class CoinList;
class CoinConstructor;

// Souls.
class SoulList;
class SoulConstructor;

// Rock piles.
class RockPileList;
class RockPileConstructor;

// Save points.
class SavePointList;
class SavePointConstructor;

// Pooled objects.
class ArrowPool;
class RockPool;

// Exit.
class LevelExit;
class LevelExitPlacer;


namespace Tmpl8
{
	class Game : public TheApp
	{
	public:
		Game();
		~Game() override;

		// Game flow.
		void Init();
		void Tick(float deltaTime);
		void Shutdown() { /* implement if you want to do something on exit */ }


		// Input handling.
		void MouseUp(int) { /* implement if you want to detect mouse button presses */ }
		void MouseDown(int) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int, int) { /* implement if you want to detect mouse position */ }
		void MouseWheel(float) { /* implement if you want to handle the mouse wheel */ }
		void KeyUp(int keycode);
		void KeyDown(int keycode);


	private:
		// Title screen.
		void RenderTitleScreen(const bool hasSaveFile);


		// Initialize persistent game objects.		
		void InitCamera();
		void InitCollisionManagers();
		void InitRenderer();

		
		// Create level.
		void ResetLevel();
		void CreateLevel(const int levelIndex);
		void PlaceObjects();

		void DeletePlayer();
		void DeleteEnemies();
		void DeleteProps();

		void InitPlayer();
		void InitEnemies();
		void InitProps();


		// Game loop.
		void ChangeState(GameState* newState);

		void StoreRewindData();
		void ApplyRewindData(const bool isFromSaveFile = false);
		void ResumeFromRewind();
		void Update(const float deltaTime);
		void CollectActiveObjects();
		void ProcessCollisions();
		void RenderGame(const float deltaTime, const bool isRewindStep = false);

		void UpdateObjects(const float deltaTime);
		void UpdateParticleSystems(const float deltaTime);

		// End screens.
		void RenderGameoverScreen(const bool hasSaveFile);
		void RenderVictoryScreen();
		void PrintScore(Surface& stage);

		// Gets and sets saved level index.
		const bool FindLevelToLoad();

		int levelCount_{ 1 };
		int levelIndex_{ 0 };


		// Friends - extensions of this class, in a way. Tightly coupled.
		// The friends will handle the order of code execution. Game will provide the methods.
		friend class TitleScreenState;
		
		friend class PlayScreenState;
		friend class NormalState;
		friend class RewindState;

		friend class EndScreenState;
		friend class GameoverState;
		friend class VictoryState;


		// The friends are states.
		GameState* state_{ nullptr };


		// Global gravity value.
		float gravity{ 300.0f };


		// Input manager - keyboard only.
		KeyboardManager* keyboardManager_{ nullptr };


		// The level and camera that views it.
		Level* level_{ nullptr };
		Camera* camera_{ nullptr };


		// Renderer will draw to the staging sprite/screen using the help of the camera, 
		// and then the stage will draw to the main screen.
		// The stage is twice as large as needed to support zooming.
		Renderer* renderer_{ nullptr };
		Sprite stagingSprite_{ new Surface{2 * TILES_PER_ROW * TILE_WIDTH, 2 * TILES_PER_COLUMN * TILE_HEIGHT}, true, 1 };
				

		// Particle system for learning, mostly, and for immersion effects.
		ParticleSystem* particleSystem_{ nullptr };


		// Collision managers.
		ObstacleCollisionManager* obstacleCM_{ nullptr };
		TileCollisionManager* tileCM_{ nullptr };
		ObjectCollisionManager* objectCM_{ nullptr };


		// Array to hold active items from all pools.
		// Passed to collision managers and renderer so time checking objects is maximized.
		GameObject* (*activeList_) { nullptr };
		int activeListCurrentIndex_{ 0 };


		// Tracks the score to draw to the HUD.
		Scorekeeper* scorekeeper_{ nullptr };


		// Stores all music. Uses an observer-subject pattern to know when to play sounds.
		AudioManager* audioManager_{ nullptr };


		// Collects and contains all data and methods needed to reverse time.
		RewindManager* rewindManager_{ nullptr };

		
		// Saves and loads the game with a json file.
		SaveSystem* saveSystem_{ nullptr };


		// Player.
		Player* player_{ nullptr };
		PlayerPlacer* playerPlacer_{ nullptr };
		

		// Archers.
		ArcherList* archerList_{ nullptr };
		ArcherConstructor* archerConstructor_{ nullptr };


		// Knights.
		KnightList* knightList_{ nullptr };
		KnightConstructor* knightConstructor_{ nullptr };
		

		// Swinging ropes.
		SwingingRopeList* ropeList_{ nullptr };
		SwingingRopeConstructor* ropeConstructor_{ nullptr };


		// Coins.
		CoinList* coinList_{ nullptr };
		CoinConstructor* coinConstructor_{ nullptr };


		// Souls.
		SoulList* soulList_{ nullptr };
		SoulConstructor* soulConstructor_{ nullptr };


		// Rock piles.
		RockPileList* rockPileList_{ nullptr };
		RockPileConstructor* rockPileConstructor_{ nullptr };


		// Save points.
		SavePointList* savePointList_{ nullptr };
		SavePointConstructor* savePointConstructor_{ nullptr };


		// Pooled objects.
		RockPool* rockPool_{ nullptr };
		ArrowPool* arrowPool_{ nullptr };

		// Exit.
		LevelExit* levelExit_{ nullptr };
		LevelExitPlacer* levelExitPlacer_{ nullptr };
	};
}