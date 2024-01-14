#pragma once
#include "menu.h"
#include <string>

Menu* InitializeMenu_func(std::string menufields[], int numOfMenuFields, std::string img, SDL_Keycode* keys, Menu* parentmenu, Menu** childmenus);
void LinkMenus_func(Menu* parentmenu, Menu** childmenus, int numOfChildMenus);
Menu* HandleMenu_func(SDL_Renderer* renderer, Gamestate &gamestate, Menu* menu, SDL_Event event, int* quit, bool* onmenu, bool renderMe);
void RenderMenu_func(SDL_Renderer *renderer, Menu* menu);
void FreeMenu_func(Menu* menu);
