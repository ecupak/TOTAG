#include "precomp.h"
#include "Game.h"

#include "RewindState.h"

#include "NormalState.h"

#include "KeyboardManager.h"
#include "RewindManager.h"
#include "AudioManager.h"


RewindState::RewindState(Game& game)
	: game_{ game }
{

}


RewindState::~RewindState()
{

}


// Events that happen when state is first assigned.
void RewindState::Enter(const bool)
{
	// Slow down and deepen pitch of the bgm.
	game_.audioManager_->AdjustSound(0.4f, 0.75f);

	// Store rewind data at the start of the frame.
	// For the rewind state, this is only done on the frame rewind mode was activated.
	// Otherwise we would be overwriting all the previous data as the player scrubs backwards.
	game_.StoreRewindData();
}


// Check input and return new state if neccessary.
GameState* RewindState::HandleInput()
{
	// To avoid any chance of the key release being missed, 
	// we check to see if the key is simply not pressed anymore.
	if (!game_.keyboardManager_->IsPressed(Action::Rewind))
	{
		return new NormalState{ game_ };
	}


	// The player can scrub back and forth over the recorded period.
	// Here we get the direction that is being pressed.
	rewindDirection_ = game_.keyboardManager_->IsPressed(Action::Left) ? -1 : 0
		+ game_.keyboardManager_->IsPressed(Action::Right) ? 1 : 0;


	// Stay in rewind mode.
	return nullptr;
}


// Apply any state actions that depend on deltaTime.
void RewindState::Update(const float deltaTime)
{
	// Only apply the rewind data for past frames upon entering the frame.
	if (rewindDirection_ != 0)
	{
		// Move the head to the new frame position.
		game_.rewindManager_->ControlHead(rewindDirection_);

		// Perform a modified game loop.

		// ... instead of Update(), apply the previous rewind data.
		// ... only get the data necessary to visualize the past frame.
		game_.ApplyRewindData();

		// ... skip collision checks.
		game_.CollectActiveObjects();
		game_.RenderGame(deltaTime, true);
	}
}


// Events that happen when state is terminated.
void RewindState::Exit()
{
	// Apply the rest of the rewind data to the objects before we return
	// to normal time.
	game_.ResumeFromRewind();
}