#include "precomp.h"
//#include "Game.h"
#include "GameState.h"

//
//GameState::GameState(Game& game)
//	: game_{ game }
//{	}


GameState::~GameState() {}


void GameState::Enter(const bool) {}


GameState* GameState::HandleInput()
{ 
	return nullptr; 
}


void GameState::Update(const float) {}


void GameState::Exit() {}