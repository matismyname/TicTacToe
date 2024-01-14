#include <SDL2/SDL.h>
#include "clog.h"
#include "constants.h"

int* GetMousePosition_func(SDL_Event event)
{
	int* coords = new int[2];
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		coords[0] = event.button.x;
		coords[1] = event.button.y;
		//Logging
		char xstring[10];
		char ystring[10];
		sprintf(xstring, "%d", coords[0]);
		sprintf(ystring, "%d", coords[1]);
		Clog("Coordinate X of mouse click: ", xstring, "DEBUG");
		Clog("Coordinate Y of mouse click: ", ystring, "DEBUG");

		return coords;
	}
	return 0;
}

int CalculateIndex_func(int point, int intervalLength)
{	
	int lowerbound = 0;
	int upperbound = CELL_W;
	Clog("The point to calculate the index is: ", point, "DEBUG");
	for (int i=0; i<TOTALROWNUM; i++)
	{
		Clog("The index for the gamestate is: ", i, "DEBUG");
		if (point >= lowerbound && point <= upperbound) { Clog("--- Done Calculating the index for the gamestate", "DEBUG"); return i; }
		Clog("The lowerbound is: ", lowerbound, "DEBUG");
		Clog("The upperbound is: ", upperbound, "DEBUG");
		lowerbound = upperbound;
		upperbound += upperbound;
	}
	Clog("Could not determine the index!", "ERROR");
	exit(EXIT_FAILURE);
}

int CalculateMidpoint_func(int point, int intervalLength)
{	
	int midp = intervalLength / 2;
	int bound = midp;
	Clog("Given point is: ", point, "DEBUG");
	for (int i=0; i<TOTALROWNUM; i++)
	{
		//Logging
		Clog("Midpoint is (LOOP): ", midp, "DEBUG");
		Clog("Distance is (LOOP): ", abs(midp - point), "DEBUG");
		//End Logging
		if (abs(midp - point) <= bound)
		{
			Clog("Returned midpoint is: ", midp, "DEBUG");
			return midp;
		}
		midp += intervalLength;
	}
	Clog("Could not calculate the Midpoint!", "DEBUG");
	exit(EXIT_FAILURE);
}
