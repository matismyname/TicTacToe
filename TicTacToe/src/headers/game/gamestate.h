#pragma once
#include "constants.h"
class Gamestate {
	public:
		bool playagainstcomputer = false;
		bool playinggame = false;
		int currentboard[TOTALROWNUM][TOTALROWNUM] = {0}; //Initialize to 0
};
