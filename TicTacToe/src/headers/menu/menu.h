#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>

struct Menu {
	std::vector<std::string> menufields;
	std::string img;
	std::vector<SDL_Keycode> keys;
	Menu* parentmenu;
	std::vector<Menu*> childmenus;
};
