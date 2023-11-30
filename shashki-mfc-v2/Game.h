#pragma once
#include "board_state.h"
#include "minimax.h"
#include "Neiron.h"
#include <iostream>
#include <sstream>
#include <string>

double* getBoard(BoardState board, Role role);
//функция, проводящая игру между неиросетями
void NeiroVsNeiro(Neiron* white, Neiron* black);

using PlayerFunction = BoardState(*)(BoardState, Neiron*);
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

BoardState playNeiron(BoardState initial, Neiron* neiro);

BoardState playNeironTrain(BoardState initial, Neiron* neiro);

//функция, которая возвращает ценность обоих игроков
void _getScore(double& s1, double& s2, BoardState initial);

//функция, которая оценивает ход 
BoardState checkNeiron(BoardState initial, Neiron* neiro);
