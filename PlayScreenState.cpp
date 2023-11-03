#include "precomp.h"
#include "Game.h"

#include "PlayScreenState.h"

#include "EndScreenState.h"

#include "NormalState.h"
#include "RewindState.h"

#include "SaveSystem.h"
#include "KeyboardManager.h"
#include "Player.h"


PlayScreenState::PlayScreenState(const bool isFromSaveFile, Game& game)
	: game_{ game }
	, isFromSaveFile_{ isFromSaveFile }
{	}


PlayScreenState::~PlayScreenState()
{
	delete state_;
}


void PlayScreenState::Enter(const bool)
{
	// Release all keys being pressed from title screen.
	game_.keyboardManager_->ReleaseAll();

	// Load level.
	game_.CreateLevel(game_.levelIndex_);
	
	if (isFromSaveFile_)
	{
		// Load saved data into rewind manager.
		game_.saveSystem_->LoadData();

		// Use rewind methods to appy the saved data to all objects.
		game_.ApplyRewindData(true);
		game_.ResumeFromRewind();
	}

	// Begin level in normal state.
	state_ = new NormalState{ game_ };
	state_->Enter();
}


GameState* PlayScreenState::HandleInput()
{
	// Perform game loop.
	GameState* state{ state_->HandleInput() };

	if (state != nullptr)
	{
		ChangeState(state);
	}

	// Check if player is dead / gameover.
	if (game_.player_->GetCurrentHP() <= 0)
	{
		return new EndScreenState{ false, game_ };
	}

	// Check if level is complete.
	if (game_.player_->IsAtLevelEnd())
	{
		// Load new level if not at the end yet.
		if (game_.levelIndex_ + 1 < game_.levelCount_)
		{
			++game_.levelIndex_;
			return new PlayScreenState{ false, game_ };
		}
		else
		{
			return new EndScreenState{ true, game_ };
		}
	}

	// Contine playing level.
	return nullptr;
}


void PlayScreenState::Update(const float deltaTime)
{
	state_->Update(deltaTime);
}


void PlayScreenState::Exit()
{

}


void PlayScreenState::ChangeState(GameState* newState)
{
	state_->Exit();

	delete state_;
	state_ = newState;

	state_->Enter();
}