#pragma once
#include <string>

class Player {
	public:
		std::string name;
		bool turn;
		bool won;
		struct color {
			uint r;
			uint b;
			uint g;
		} playerColor;
	Player(	const std::string& playerName,
		bool playerTurn,
		bool playerWon,
		uint colorR,
		uint colorB,
		uint colorG)
        	: name(playerName), turn(playerTurn), won(playerWon), playerColor{colorR, colorB, colorG} {}

};
