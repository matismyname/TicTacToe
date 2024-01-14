#include <SDL2/SDL.h>
#include "clog.h"
#include "constants.h"
#include "gamestate.h"
#include "helperfuncs.h"
#include "menu.h"
#include "player.h"
#include "gamelogic.h"
#include "gamerender.h"
#include <string>
#include <cstring>

void HandleGame_func(SDL_Renderer *renderer, Gamestate &gamestate, Player players[2], int* playernum, Menu* menu, SDL_Event event, int* quit, bool* onmenu)
{
	int renderMoveFlag;

	switch (event.type)
	{
		case SDL_QUIT: {
			*quit = 1;
			Clog("The game has been quit!", "DEBUG");
			       } break;
		case SDL_MOUSEBUTTONDOWN: {
			//If the game is over, do nothing
			if (!gamestate.playinggame)
				break;

			int* coords = GetMousePosition_func(event);
			renderMoveFlag = RenderMove_func(renderer, players[*playernum], gamestate, coords[0], coords[1]);
			CheckIfGameOver_func(renderer, gamestate, players[*playernum].name);

			Clog("Playernum is: ", *playernum, "DEBUG"); //Logging
			if (*playernum == 0 && !renderMoveFlag)
			{
				players[*playernum].turn = false;
				*playernum = 1;
				players[*playernum].turn = true;
			}
			else if (*playernum == 1 && !renderMoveFlag)
			{
				players[*playernum].turn = false;
				*playernum = 0;
				players[*playernum].turn = true;
			}
					  } break;
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym)
			{
				case SDLK_q: {
					*quit = 1;
					Clog("The game has been quit!", "DEBUG");
					     } break;
				case SDLK_n: {
					Clog("The game has been reset!", "DEBUG");
					for (int i=0; i<TOTALROWNUM; i++)
					{
						for (int j=0; j<TOTALROWNUM; j++)
						{
							gamestate.currentboard[i][j] = 0;
						}
					}
					gamestate.playinggame = true;
					RenderGrid_func(renderer);
					     } break;
				case SDLK_b: {
						*onmenu = true;			     
					     } break;
			}
				  } break;
	}
}

void CheckIfGameOver_func(SDL_Renderer* renderer, Gamestate &gamestate, std::string name)
{
	int playerNum = 0;
	char* msg;
	int sum = 0;
	int cellNum = 0;
	bool gameover = false;
	int victoryboardidx[TOTALROWNUM][TOTALROWNUM] = {0};

	if (name == "Player 1") { playerNum = 1; }
	else if (name == "Player 2") { playerNum = 2; }

	gameover = CheckIfGameOver_func(gamestate, playerNum, victoryboardidx); //Check rows
	Clog("----- Game Over Condition Checked For Player: ", playerNum, "DEBUG");
	if (gameover) { RenderVictoryScreen_func(victoryboardidx, renderer, name); }
}

bool CheckIfGameOver_func(Gamestate &gamestate, int playernum, int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM])
{
	if (CheckIfGameOverRow_func(gamestate, playernum, victoryboardidx)) return true; //Same algo as next one, but I duplicate the code for modularity
	else if (CheckIfGameOverColumn_func(gamestate, playernum, victoryboardidx)) return true;
	if (CheckIfGameOverDiagonal_func(gamestate, playernum, victoryboardidx)) return true;
	return false;
}

bool CheckIfGameOverRow_func(Gamestate &gamestate, int playernum, int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM])
{
	int sum = 0;
	int cellNum = 0;

	//Go through each row
	for (int k=0; k<TOTALROWNUM; k++)
	{
		for (int i=0; i<TOTALROWNUM; i++)
		{
			cellNum = gamestate.currentboard[k][i];

			Clog("Cell num is: ", cellNum, "DEBUG");
			Clog("i is: ", i, "DEBUG");
			if (cellNum == playernum)
			{
				sum += cellNum;
				victoryboardidx[k][i] = cellNum;
			}
		}

		Clog("The sum of the gameboard is: ", sum, "DEBUG"); //Logging
		Clog("The multiplication results in: ", TOTALROWNUM*playernum, "DEBUG"); //Logging

		//If the player won, the game ends
		if (sum == TOTALROWNUM*playernum)
		{
			gamestate.playinggame = false;
			return true;
		}
		victoryboardidx[TOTALROWNUM][TOTALROWNUM] = {0};
		sum = 0;
	}
	return false;
}

bool CheckIfGameOverColumn_func(Gamestate &gamestate, int playernum, int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM])
{
	int sum = 0;
	int cellNum = 0;

	//Go through each row
	for (int k=0; k<TOTALROWNUM; k++)
	{
		for (int i=0; i<TOTALROWNUM; i++)
		{
			cellNum = gamestate.currentboard[i][k];

			Clog("Cell num is: ", cellNum, "DEBUG");
			Clog("k is: ", k, "DEBUG");
			if (cellNum == playernum)
			{
				victoryboardidx[i][k] = cellNum;
				sum += cellNum;
			}
		}

		Clog("The sum of the gameboard is: ", sum, "DEBUG"); //Logging
		Clog("The multiplication results in: ", TOTALROWNUM*playernum, "DEBUG"); //Logging

		//If the player won, the game ends
		if (sum == TOTALROWNUM*playernum)
		{
			gamestate.playinggame = false;
			return true;
		}
		victoryboardidx[TOTALROWNUM][TOTALROWNUM] = {0};
		sum = 0;
	}
	return false;
}

bool CheckIfGameOverDiagonal_func(Gamestate &gamestate, int playernum, int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM])
{
	int sum = 0;

	for (int i=0; i<TOTALROWNUM; i++)
	{
		//Make sure to not count the other player's moves
		if (gamestate.currentboard[i][i] == playernum)
		{
			sum += gamestate.currentboard[i][i];
			Clog("The sum for the first diagonal is: ", sum, "DEBUG");
			victoryboardidx[i][i] = gamestate.currentboard[i][i];
		}
	}
	Clog("--- Done checking the first diagonal", "DEBUG");

	//If the player won, the game ends
	if (sum == TOTALROWNUM*playernum)
	{
		gamestate.playinggame = false;
		return true;
	}
	victoryboardidx[TOTALROWNUM][TOTALROWNUM] = {0};
	sum = 0;

	for (int i=0; i<TOTALROWNUM; i++)
	{
		//Make sure to not count the other player's moves
		if (gamestate.currentboard[i][TOTALROWNUM-1-i] == playernum)
		{
			sum += gamestate.currentboard[i][TOTALROWNUM-1-i];
			Clog("The sum for the first diagonal is: ", sum, "DEBUG");
			victoryboardidx[i][TOTALROWNUM-1-i] = gamestate.currentboard[i][TOTALROWNUM-1-i];
		}
	}	
	Clog("--- Done checking the second diagonal", "DEBUG");

	//If the player won, the game ends
	if (sum == TOTALROWNUM*playernum)
	{
		gamestate.playinggame = false;
		return true;
	}
	victoryboardidx[TOTALROWNUM][TOTALROWNUM] = {0};

	return false;
}

void ComputerPlays_func(SDL_Renderer* renderer, Gamestate &gamestate, Player players[2], int* playernum)
{
}

