#include "precomp.h"
#include "Game.h"
#include "NormalPlayState.h"

#include "KeyboardManager.h"
#include "AudioManager.h"
#include "RewindManager.h"

#include "RewindPlayState.h"


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
NormalPlayState::NormalPlayState(Game& game)
	: GameState{ game }
{	}


NormalPlayState::~NormalPlayState() {}



void NormalPlayState::Enter(const bool deltaTime) 
{
	game_.audioManager_->AdjustSound(1.0f, 1.0f);
}


GameState* NormalPlayState::HandleInput()
{
	if (game_.keyboardManager_->IsJustPressed(Action::Rewind))
	{
		return new RewindPlayState{ game_ };
	}

	return nullptr;
}


void NormalPlayState::Update(const float deltaTime)
{
	// Store start of frame as rewind data.
	game_.StoreRewindData();
	
	// Advance rewind to next frame to fill with data.
	game_.rewindManager_->AdvanceHead();

	// Normal loop.
	game_.Update(deltaTime);
	game_.CollectActiveObjects();
	game_.ProcessCollisions();
	game_.RenderGame();
}


void NormalPlayState::Exit() {}