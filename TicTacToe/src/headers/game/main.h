#pragma once
#include "player.h"
#include "menu.h"
#include "gamestate.h"

SDL_Window* InitializeWindow_func();
SDL_Renderer* InitializeRenderer_func(SDL_Window *window);
void RunGame_func(SDL_Renderer* renderer, Gamestate &gamestate, Player players[2], Menu* menu);
