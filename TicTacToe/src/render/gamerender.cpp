#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "clog.h"
#include "player.h"
#include "gamelogic.h"
#include "helperfuncs.h"
#include "gamerender.h"
#include "menu.h"

//This function renders the grid lines
void RenderGrid_func(SDL_Renderer *renderer)
{
	//White background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i=1; i<TOTALROWNUM; i++)
	{
		Clog("Drawing gridlines...", "DEBUG");
		//Vertical lines
		SDL_RenderDrawLine(renderer,
				i*CELL_W, 0,
				i*CELL_W, SCREEN_H);
		//Horizontal lines
		SDL_RenderDrawLine(renderer,
				0, i*CELL_H,
				SCREEN_W, i*CELL_H);
		Clog("Gridlines successfully drawn!", "DEBUG");
	}
	SDL_RenderPresent(renderer);
}

int RenderMove_func(SDL_Renderer *renderer, Player &player, Gamestate &gamestate, int xcord, int ycord) 
{
	SDL_SetRenderDrawColor(renderer,
			player.playerColor.r,
			player.playerColor.g,
			player.playerColor.b,
			255);
	Clog("Xcord is: ", xcord, "DEBUG");
	Clog("Ycord is: ", ycord, "DEBUG");
	//Calculate the indices for the gamestateboard
	int idx = CalculateIndex_func(ycord, CELL_W);
	int idy = CalculateIndex_func(xcord, CELL_H);
	//Calculate the midpoints for the gamestateboard
	int midx = CalculateMidpoint_func(xcord, CELL_W);
	int midy = CalculateMidpoint_func(ycord, CELL_H);
	Clog("Idx is: ", idx, "DEBUG");
	Clog("Idy is: ", idy, "DEBUG");

	//If the move is not valid
	if (gamestate.currentboard[idx][idy] != 0)
		return 1;
	int legLength = 70; //a²+b² = c²
	//Draw X
	if (player.name == "Player 1")
	{
		gamestate.currentboard[idx][idy] = 1;
		RenderX_func(renderer, midx, midy, legLength);
	}
	//Draw O
	else
	{
		gamestate.currentboard[idx][idy] = 2;
		for (int i=0; i<4 ;i++)
			RenderO_func(renderer, midx, midy, i);
	}
	SDL_RenderPresent(renderer);
	//If the move was successfully rendered, return 0
	return 0;
}

void RenderX_func(SDL_Renderer *renderer, int midx, int midy, int legLength)
{
	//For the X legs
	int startx = 0;
	int starty = 0;
	int endx = 0;
	int endy = 0;

	Clog("MidX is: ", midx, "DEBUG");
	Clog("MidY is: ", midy, "DEBUG");

	//First leg of the X
	startx = midx - legLength;	
	starty = midy - legLength;
	endx = midx + legLength;
	endy = midy + legLength;
	Clog("StartX is: ", startx, "DEBUG");
	Clog("StartY is: ", starty, "DEBUG");
	Clog("EndX is: ", endx, "DEBUG");
	Clog("EndY is: ", endy, "DEBUG");
	for (int i=0; i<5; i++)
	{
		SDL_RenderDrawLine(renderer, startx + i, starty, endx + i, endy);
	}

	//Second leg of the X
	startx = midx + legLength;	
	starty = midy - legLength;
	endx = midx - legLength;
	endy = midy + legLength;
	for (int i=0; i<5; i++)
	{
		SDL_RenderDrawLine(renderer, startx + i, starty, endx + i, endy);
	}
}

//Midpoint Cirlce Algorithm
void RenderO_func(SDL_Renderer *renderer, int midx, int midy, int decrease)
{
	int16_t radius = 70 - decrease;
	const int32_t diam = (radius * 2);
	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t err = (tx - diam);

	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, midx + x, midy - y);
		SDL_RenderDrawPoint(renderer, midx + x, midy + y);
		SDL_RenderDrawPoint(renderer, midx - x, midy - y);
		SDL_RenderDrawPoint(renderer, midx - x, midy + y);
		SDL_RenderDrawPoint(renderer, midx + y, midy - x);
		SDL_RenderDrawPoint(renderer, midx + y, midy + x);
		SDL_RenderDrawPoint(renderer, midx - y, midy - x);
		SDL_RenderDrawPoint(renderer, midx - y, midy + x);
		if (err <= 0)
		{
			++y;
			err += ty;
			ty += 2;
		}

		if (err > 0)
		{
			--x;
			tx += 2;
			err += (tx - diam);
		}
	}
}

void RenderMenu_func(SDL_Renderer *renderer, Menu* menu)
{
	SDL_Surface* imageSurface = IMG_Load(menu->img.c_str());
	if (!imageSurface)
	{
		Clog("IMG_Load: ", IMG_GetError(), "ERROR");
		exit(EXIT_FAILURE);
	}
    	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    	SDL_FreeSurface(imageSurface);

    	SDL_Rect destinationRect = { 0, 0, SCREEN_H, SCREEN_W };
	SDL_RenderClear(renderer);
    	SDL_RenderCopy(renderer, imageTexture, NULL, &destinationRect);
    	SDL_RenderPresent(renderer);
}

void RenderVictoryScreen_func(int (&victoryboardidx)[TOTALROWNUM][TOTALROWNUM], SDL_Renderer* renderer, std::string name)
{	
	Clog("--- RENDERING VICTORY BOARD ---", "DEBUG");
	SDL_SetRenderDrawColor(renderer,
			0,
			0,
			0,
			255);
	Gamestate mockgs;
	Player mockplayer(name, false, false, 0, 0, 0);
	int midpx = 0;
	int midpy = 0;
	int val = 0;
	RenderGrid_func(renderer);
	for (int i=0; i<TOTALROWNUM; i++)
	{
		midpy = (CELL_H / 2) + (i*CELL_H);
		Clog("Midpy is: ", midpy, "DEBUG");
		for (int k=0; k<TOTALROWNUM; k++)
		{
			midpx = (CELL_H / 2) + (k*CELL_H);
			Clog("Midpx is: ", midpx, "DEBUG");
			val = victoryboardidx[i][k];
			Clog("The value of the victoryboard is: ", val, "DEBUG");
			if (val != 0)
			{
				Clog("Midpx to render: ", midpx, "DEBUG");
				Clog("Midpy to render: ", midpy, "DEBUG");
				RenderMove_func(renderer, mockplayer, mockgs, midpx, midpy);
				Clog("--- Victory screen point rendered", "DEBUG");
			}
		}
	}
	for (int i=0; i<TOTALROWNUM; i++)
	{
		for (int k=0; k<TOTALROWNUM; k++)
		{
			victoryboardidx[i][k] = 0;
		}
	}
	Clog("--- END RENDERING VICTORY BOARD ---", "DEBUG");
}
