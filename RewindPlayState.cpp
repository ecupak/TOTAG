#include "precomp.h"
#include "Game.h"
#include "RewindPlayState.h"

#include "KeyboardManager.h"
#include "AudioManager.h"
#include "RewindManager.h"

#include "NormalPlayState.h"


// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.
RewindPlayState::RewindPlayState(Game& game)
	: GameState{ game }
{	}


RewindPlayState::~RewindPlayState() {}



void RewindPlayState::Enter(const bool deltaTime)
{
	// Rewind data is stored once here so the visible frame is accessible to the player by scrubbing the history.
	// Otherwise, the visible frame that the rewind mode starts on is inaccessible (it does not get saved to rewind manager).
	game_.StoreRewindData();

	game_.audioManager_->AdjustSound(1.0f, 0.75f);
}


GameState* RewindPlayState::HandleInput()
{
	if (game_.keyboardManager_->IsJustReleased(Action::Rewind))
	{
		return new NormalPlayState{ game_ };
	}
	else
	{
		rewindDirection_ =
			game_.keyboardManager_->IsPressed(Action::Left) 
				? -1 : 0
			+ game_.keyboardManager_->IsPressed(Action::Right) 
				? 1 : 0;

		return nullptr;
	}
}


void RewindPlayState::Update(const float deltaTime)
{
	if (rewindDirection_ != 0)
	{
		// Move the head based on input received.
		game_.rewindManager_->ControlHead(rewindDirection_);

		// Apply the rewind data of the head to objects.
		// Only applies data needed to vizualize frame.
		// Only apply when the head is moved.
		game_.ApplyRewindData();
	}

	// Rest of normal loop. Skip collision checks.
	game_.CollectActiveObjects();
	game_.RenderGame();
}


void RewindPlayState::Exit()
{
	// Apply the rest of the rewind data of the head to objects.
	game_.ResumeFromRewind();

	// Mark this point as the new head (cannot press rewind again and go into a future that does not exist).
	game_.rewindManager_->ResetHead();
}