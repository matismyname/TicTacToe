#pragma once
#include "player.h"
#include "gamestate.h"
#include <string>

void RenderGrid_func(SDL_Renderer *renderer);
int RenderMove_func(SDL_Renderer *renderer, Player &player, Gamestate &gamestate, int x, int y);
void RenderX_func(SDL_Renderer *renderer, int midx, int midy, int legLength);
void RenderO_func(SDL_Renderer *renderer, int midx, int midy, int decrease);
void RenderMenu_func(SDL_Renderer *renderer);
void RenderVictoryScreen_func(int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM], SDL_Renderer* renderer, std::string name);
