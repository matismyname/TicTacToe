#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <string>
#include "clog.h"
#include "constants.h"
#include "gamelogic.h"
#include "menulogic.h"
#include "gamerender.h"
#include "player.h"
#include "gamestate.h"
#include "menu.h"
#include "main.h"
#include "initSDL.h"
#include <unistd.h>

int main()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		char* msg = CombineMessages_func("Could not initialize sdl2: ", SDL_GetError());
		Clog(msg, "ERROR");
		return EXIT_FAILURE;
	}

	SDL_Window* window = InitializeWindow_func();
	SDL_Renderer* renderer = InitializeRenderer_func(window);

	// -- Initialize Menus --
	std::string mainmenufields[] = {"Play TicTacToe (P)", "Quit (Q)"};
	SDL_Keycode mainmenukeys[] = {SDLK_p, SDLK_q};
	Menu* parentmenu = nullptr; //The mainmenu does not have a parent menu
	Menu* childmenusOfMainMenu[] = {nullptr, nullptr}; //Quit does not have any child menus
	std::string mainmenuimg(MAINMENUIMG);
	Menu* mainmenu = InitializeMenu_func(mainmenufields, 2, mainmenuimg, mainmenukeys, parentmenu, childmenusOfMainMenu);

	std::string playagainstmenufields[] = {"Play against Human (H)", "Play against Computer (C)", "Go back (B)","Quit (Q)"};
	SDL_Keycode playagainstmenukeys[] = {SDLK_h, SDLK_c, SDLK_b, SDLK_q};
	Menu* childmenusOfPlayAgainstMenu[] = {nullptr, nullptr, nullptr, nullptr};
	std::string playagainstmenuimg(PLAYAGAINSTMENUIMG);
	Menu* playagainstmenu = InitializeMenu_func(playagainstmenufields, 4, playagainstmenuimg, playagainstmenukeys, parentmenu, childmenusOfPlayAgainstMenu);

	childmenusOfMainMenu[0] = playagainstmenu;
	LinkMenus_func(mainmenu, childmenusOfMainMenu, 2); //Links parent-child menus

	// -- Initialize Players --
	Player player1("Player 1", false, false, 0, 0, 255);
	Player player2("Player 2", false, false, 255, 0, 0);
	Player players[2] = {player1, player2};

	// -- Initialize Gamestate --
	Gamestate gamestate;

	for (int i=0; i<TOTALROWNUM; i++)
	{
		for (int k=0; k<TOTALROWNUM; k++)
		{
			if (gamestate.currentboard[i][k] != 0)
			{
				Clog("The board is not initialized to zero!", "DEBUG");
				exit(EXIT_FAILURE);
			}
		}
	}

	//Render the main menu
	RenderMenu_func(renderer, mainmenu);

	//Run the game loop
	RunGame_func(renderer, gamestate, players, mainmenu);

	//Cleanup
	FreeMenu_func(mainmenu); //Frees all its children too
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

//Return 1 if the game is quit
void RunGame_func(SDL_Renderer* renderer, Gamestate& gamestate, Player players[2], Menu* mainmenu)
{
	SDL_Event event;
	int quit = 0;
	int playernum = 0;
	bool onmenu = true;
	Menu* menu = mainmenu;

	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			if (quit)
			{
				return;
			}

			if (onmenu)
			{
				menu = HandleMenu_func(renderer, gamestate, menu, event, &quit, &onmenu, false);
				if (!onmenu)
				{
					RenderGrid_func(renderer);
				}
			}
			else
			{
				HandleGame_func(renderer, gamestate, players, &playernum, menu, event, &quit, &onmenu);
				if (onmenu)
				{
					menu = HandleMenu_func(renderer, gamestate, menu, event, &quit, &onmenu, true); //Renders the menu as well
				}
			}
		}
		sleep(0.6);
	}
}

