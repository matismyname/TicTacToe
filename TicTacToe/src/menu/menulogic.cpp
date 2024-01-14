#include "menu.h"
#include "clog.h"
#include "gamestate.h"
#include "gamerender.h"
#include "menulogic.h"
#include <SDL2/SDL.h>
#include <string>

Menu* InitializeMenu_func(std::string menufields[], int numOfMenuFields, std::string img, SDL_Keycode* keys, Menu* parentmenu, Menu** childmenus)
{
    Menu* newMenu = new Menu;

    // Copy menufields
    newMenu->menufields.reserve(numOfMenuFields);
    for (int i = 0; i < numOfMenuFields; i++) {
        newMenu->menufields.push_back(menufields[i]);
    }

    newMenu->img = img;

    // Copy keys
    newMenu->keys.reserve(numOfMenuFields);
    std::copy(keys, keys + numOfMenuFields, std::back_inserter(newMenu->keys));

    // Set parentmenu and childmenus
    newMenu->parentmenu = parentmenu;

    for (int i=0; i<numOfMenuFields; i++)
    {
	    newMenu->childmenus.push_back(childmenus[i]);
    }

    return newMenu;
}

void LinkMenus_func(Menu* parentmenu, Menu** childmenus, int numOfChildMenus)
{
	for (int i=0; i<numOfChildMenus; i++)
	{
		if (childmenus[i] == nullptr)
			break;
		childmenus[i]->parentmenu = parentmenu;
		parentmenu->childmenus[i] = childmenus[i];
	}
}

Menu* HandleMenu_func(SDL_Renderer* renderer, Gamestate &gamestate, Menu* menu, SDL_Event event, int* quit, bool* onmenu, bool renderMe)
{
	//If the player didn't press any key
	if (event.type != SDL_KEYDOWN)
		return menu;

	//If the player quits the game
	if (event.key.keysym.sym == SDLK_q)
	{
		*quit = 1;
		return nullptr;
	}
	//To handle the case when the game is being played but the player wants to go back to the last menu
	if (renderMe == true)
	{
		RenderMenu_func(renderer, menu);
		return menu;
	}

	int numOfKeys = menu->keys.size();
	for (int i=0; i<numOfKeys; i++)
	{
		if (event.key.keysym.sym == menu->keys[i])
		{	
			if (menu->keys[i] == SDLK_b && menu->parentmenu != nullptr)
			{
				RenderMenu_func(renderer, menu->parentmenu);
				return menu->parentmenu;
			}
			if (menu->childmenus[i] != nullptr)
			{
				RenderMenu_func(renderer, menu->childmenus[i]);
				return menu->childmenus[i];
			}

			gamestate.playinggame = true;
			*onmenu = false;
			if (event.key.keysym.sym == SDLK_c)
			{
				gamestate.playagainstcomputer = true;
			}
		}
	}
	return menu;
}

void FreeMenu_func(Menu* menu)
{
	if (menu == nullptr)
		return;

	for (int i=0; i<menu->childmenus.size(); i++)
	{
		FreeMenu_func(menu->childmenus[i]);
	}

	menu->menufields.clear();
	menu->keys.clear();
	menu->childmenus.clear();
	delete menu;
	Clog("Menu succesfully freed!", "DEBUG");
}



