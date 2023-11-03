#include "precomp.h"
#include "Game.h"

#include "NormalState.h"


#include "RewindState.h"

#include "KeyboardManager.h"
#include "RewindManager.h"
#include "AudioManager.h"

#include "SaveSystem.h"


NormalState::NormalState(Game& game)
	: game_{ game }
{

}


NormalState::~NormalState()
{

}


void NormalState::Enter(const bool)
{
	// Restore the pitch of the bgm.
	game_.audioManager_->AdjustSound(0.6f, 1.0f);
}


GameState* NormalState::HandleInput()
{
	// To avoid any chance of the key press being missed, 
	// we check to see if the key is simply pressed at all.
	if (game_.keyboardManager_->IsPressed(Action::Rewind))
	{
		return new RewindState{ game_ };
	}


	// Stay in normal mode.
	return nullptr;
}


void NormalState::Update(const float deltaTime)
{
	// Store rewind data at the start of each frame.
	// When the player rewinds to this frame, they will start it the same way every time.
	game_.StoreRewindData();


// Save before head advances to an empty (or old data) spot.
// Load
#ifdef _DEBUG
	if (game_.keyboardManager_->IsJustPressed(Action::Save))
	{
		game_.saveSystem_->SaveData();
	}

	if (game_.keyboardManager_->IsJustPressed(Action::Load))
	{
		game_.saveSystem_->LoadData();

		// Apply rewind data and then resume the game.
		game_.ApplyRewindData(true);
		game_.ResumeFromRewind();
	}
#endif


	// Perform a regular game loop.
	game_.Update(deltaTime);
	game_.CollectActiveObjects();
	game_.ProcessCollisions();
	game_.RenderGame(deltaTime);


	// Then advance the head to be ready for next frame.
	// This happens after the storage step, so that the first frame of the game
	// does not contain empty data - letting the player rewind into that would be bad.
	game_.rewindManager_->AdvanceHead();
}


void NormalState::Exit()
{

}