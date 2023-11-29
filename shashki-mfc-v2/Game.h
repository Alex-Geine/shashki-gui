#pragma once
#include "board_state.h"
#include "minimax.h"

#include <iostream>
#include <sstream>
#include <string>

using PlayerFunction = BoardState(*)(BoardState);
double** GetBoardMas(BoardState board);
void printBoard(BoardState board);

Direction parseDirection(std::string word, Role color);

int parseInteger(std::string word);

class MotionException {
public:
	MotionException(std::string s, bool p = false) : str(s), sad_print(p) {}
	std::string str;
	bool sad_print;
};

BoardState hungryMotion(BoardState initial, std::istringstream& in, int bufsize);

BoardState quietMotion(BoardState initial, std::istringstream& in);

BoardState playHuman(BoardState initial, std::string Step);

BoardState playAutomatic(BoardState initial);

