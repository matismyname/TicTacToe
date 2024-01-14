#pragma once
#include "gamestate.h"
#include "player.h"
#include "menu.h"
#include <string>

void CheckIfGameOver_func(SDL_Renderer* renderer, Gamestate &gamestate, std::string name);
void HandleGame_func(SDL_Renderer* renderer, Gamestate &gamestate, Player players[2], int* playernum, Menu* menu, SDL_Event event, int* quit, bool* onmenu);
bool CheckIfGameOver_func(Gamestate &gamestate, int playernum, int (&playernumvictoryboardidx)[TOTALROWNUM][TOTALROWNUM]);
bool CheckIfGameOverRow_func(Gamestate &gamestate, int playernum, int (&playernumvictoryboardidx)[TOTALROWNUM][TOTALROWNUM]);
bool CheckIfGameOverColumn_func(Gamestate &gamestate, int playernum, int (&playernumvictoryboardidx)[TOTALROWNUM][TOTALROWNUM]);
bool CheckIfGameOverDiagonal_func(Gamestate &gamestate, int playernum, int (&playernumvictoryboardidx)[TOTALROWNUM][TOTALROWNUM]);
void ComputerPlays_func(SDL_Renderer* renderer, Gamestate &gamestate, Player players[2], int* playernum);
