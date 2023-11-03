#include "precomp.h"
#include "Game.h"

#include "VictoryState.h"

#include "TitleScreenState.h"
#include "PlayScreenState.h"

#include "KeyboardManager.h"
#include "AudioManager.h"


VictoryState::VictoryState(Game& game)
	: game_{ game }
{

}


VictoryState::~VictoryState()
{

}


// Events that happen when state is first assigned.
void VictoryState::Enter(const bool)
{
	game_.audioManager_->FadeOut();

	game_.RenderVictoryScreen();
}


// Check input and return new state if neccessary.
GameState* VictoryState::HandleInput()
{
	if (game_.keyboardManager_->IsJustPressed(Action::Jump))
	{
		return new TitleScreenState{ game_ };
	}
	else if (game_.keyboardManager_->IsJustPressed(Action::Ability))
	{
		return new PlayScreenState{ false, game_ };		
	}

	return nullptr;
}


// Apply any state actions that depend on deltaTime.
void VictoryState::Update(const float)
{
	
}


// Events that happen when state is terminated.
void VictoryState::Exit()
{

}