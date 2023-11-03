#include "precomp.h"
#include "Game.h"
#include "EndScreenState.h"


#include "VictoryState.h"
#include "GameoverState.h"

#include "KeyboardManager.h"
#include "AudioManager.h"


// [Credit] https://gameprogrammingpatterns.com/state.html
// Inspiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
EndScreenState::EndScreenState(const bool isVictoryScreen, Game& game)
	: game_{ game }
{	
	if (isVictoryScreen)
	{
		state_ = new VictoryState{ game_ };
	}
	else
	{
		state_ = new GameoverState{ game_ };
	}
}


EndScreenState::~EndScreenState() {}



void EndScreenState::Enter(const bool)
{
	state_->Enter();

	//game_.audioManager_->AdjustSound(1.0f, 1.0f);
}


GameState* EndScreenState::HandleInput()
{
	// Victory and Gameover will handle their menus.
	// They will return a PlayScreenState(0 / loaded) or a TitleScreenState.
	// Otherwise, a nullptr will be returned until a decision is made.
	return state_->HandleInput();
}


void EndScreenState::Update(const float deltaTime)
{
	state_->Update(deltaTime);
}


void EndScreenState::Exit()
{
	//game_.audioManager_->AdjustSound(1.0f, 1.0f);
}