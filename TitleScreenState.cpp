#include "precomp.h"
#include "Game.h"

#include "TitleScreenState.h"

#include "KeyboardManager.h"
#include "AudioManager.h"

#include "PlayScreenState.h"


// [Credit] https://gameprogrammingpatterns.com/state.html
// Inspiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
TitleScreenState::TitleScreenState(Game& game)
	: game_{ game }
{	}


TitleScreenState::~TitleScreenState() {}



void TitleScreenState::Enter(const bool)
{
	//game_.audioManager_->AdjustSound(1.0f, 1.0f);

	// Check if save file exists.
	ifstream saveFile("assets/save_game.json");

	game_.RenderTitleScreen(!saveFile.fail());
}


GameState* TitleScreenState::HandleInput()
{ 
	if (game_.keyboardManager_->IsJustPressed(Action::Jump))
	{
		return new PlayScreenState{false, game_ };
	}
	else if (game_.keyboardManager_->IsJustPressed(Action::Throw))
	{
		if (game_.FindLevelToLoad())
		{
			return new PlayScreenState{ true, game_ };
		}
	}

	return nullptr;
}


void TitleScreenState::Update(const float)
{
	
}


void TitleScreenState::Exit() 
{
	//game_.audioManager_->AdjustSound(1.0f, 1.0f);
}