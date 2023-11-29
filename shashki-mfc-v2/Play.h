#pragma once
#include "board_state.h"
#include "minimax.h"
#include <iostream>
#include <sstream>
#include <string>

double** GetBoard(BoardState board) {
	//массив значений на доске
	double** res = new double* [8];
	for (int i = 0; i < 8; i++)
		res[i] = new double[8];

	Position p = board.position();


	for (int r = 0; r < 8; r++) {

		for (int c = 0; c <= 7; ++c) {
			Cell cell(c, r);
			if (cell.valid()) {
				if (p.ghost(cell))
					res[r][c] = 0;		//сьеденная шашка
				else if (p.color(cell) == Role::None)
					res[r][c] = 0;		//пустая шашка
				else {


					res[c][r] = p.color(cell) == Role::White ? 1 : -1;
					if (p.king(cell))
						res[c][r] *= 2;			//если дамка

				}
			}

		}

	}


	return res;
}
//
//void printBoard(BoardState board) {
//	Position p = board.position();
//	std::cout << "\n\n      *********************************\n";
//	for (int r = 7; r >= 0; --r) {
//		unsigned char n = r + '1';
//		std::cout << "    " << n << " *";
//		for (int c = 0; c <= 7; ++c) {
//			Cell cell(c, r);
//			if (cell.valid()) {
//				if (p.ghost(cell))
//					std::cout << "***";
//				else if (p.color(cell) == Role::None)
//					std::cout << "///";
//				else {
//					char buffer[4];
//					buffer[3] = '\0';
//					for (int i = 0; i < 3; ++i)
//						buffer[i] = p.color(cell) == Role::White ? 'w' : 'b';
//					if (p.king(cell))
//						buffer[1] = 'k';
//					if (board.color() == p.color(cell))
//						for (int i = 0; i < 3; ++i)
//							buffer[i] += ('A' - 'a');
//					std::cout << buffer;
//				}
//			}
//			else
//				std::cout << "   ";
//			if (c != 7)
//				std::cout << '|';
//		}
//		std::cout << "*\n";
//		if (r != 0)
//			std::cout << "      *---+---+---+---+---+---+---+---*\n";
//	}
//	std::cout << "      *********************************\n";
//	std::cout << "        A   B   C   D   E   F   G   H  \n\n";
//}
//
//Direction parseDirection(std::string word, Role color) {
//	if (color == Role::White) {
//		if (word == "left") return Direction::LeftForward;
//		if (word == "right") return Direction::RightForward;
//		if (word == "left-back") return Direction::LeftBackward;
//		if (word == "right-back") return Direction::RightBackward;
//	}
//	if (color == Role::Black) {
//		if (word == "left") return Direction::LeftBackward;
//		if (word == "right") return Direction::RightBackward;
//		if (word == "left-back") return Direction::LeftForward;
//		if (word == "right-back") return Direction::RightForward;
//	}
//	return Direction::None;
//}
//
//int parseInteger(std::string word) {
//	int n = 0;
//	for (char c : word) {
//		if (c >= '0' && c <= '9')
//			n = (c - '0') + n * 10;
//		else
//			return -1;
//	}
//	return n;
//}
//
//class MotionException {
//public:
//	MotionException(std::string s, bool p = false) : str(s), sad_print(p) {}
//	std::string str;
//	bool sad_print;
//};
//
//BoardState hungryMotion(BoardState initial, std::istringstream& in, int bufsize) {
//	std::string buf;
//	bool sad_print = false;
//	while (true) {
//		std::string w;
//		in >> w;
//		Direction d = parseDirection(w, initial.color());
//		bool king = initial.position().king(initial.place());
//		if (!initial.control(initial.place().neighbour(d)))
//			throw MotionException("wrong direction (in hungry)", sad_print);
//		while (!initial.capture())
//			initial.control(initial.place().neighbour(d));
//		int count = 1;
//		if (king) {
//			in >> w;
//			count = parseInteger(w);
//			if (count <= 0)
//				throw MotionException("no motion count (in hungry)");
//		}
//		while (count--) {
//			if (!initial.control(initial.place().neighbour(d)))
//				throw MotionException("the motion count is too high (in hungry)");
//		}
//		if (initial.control(Cell()))
//			return initial;
//		int g = in.tellg();
//		if (g == bufsize || g < 0) {
//			sad_print = true;
//			printBoard(initial);
//			std::cout << "then: ";
//			std::getline(std::cin, buf);
//			in.str(buf);
//			in.clear();
//			bufsize = buf.size();
//		}
//	}
//}
//
//BoardState quietMotion(BoardState initial, std::istringstream& in) {
//	std::string w;
//	in >> w;
//	Direction d = parseDirection(w, initial.color());
//	bool king = initial.position().king(initial.place());
//	if (!initial.control(initial.place().neighbour(d)))
//		throw MotionException("wrong direction (in quiet)");
//	if (king) {
//		in >> w;
//		int count = parseInteger(w);
//		if (count <= 0)
//			throw MotionException("no motion count (in quiet)");
//		while (--count) {
//			if (!initial.control(initial.place().neighbour(d)))
//				throw MotionException("the motion count is too high (in quiet)");
//		}
//	}
//	initial.control(Cell());
//	return initial;
//}
//
//BoardState playHuman(BoardState initial) {
//	if (initial.lost() || !initial.finished())
//		return BoardState();
//	BoardState board;
//	bool quiet = initial.quiet();
//	bool complete;
//	do {
//		complete = true;
//		try {
//			std::cout << "Your move: ";
//			std::string buf;
//			std::getline(std::cin, buf);
//			std::istringstream in(buf);
//			std::string w;
//			in >> w;
//			if (w == "quit")
//				return BoardState();
//			Cell cell = Cell::fromString(w);
//			board = initial;
//			if (!board.control(cell))
//				throw MotionException("wrong cell");
//			if (quiet)
//				board = quietMotion(board, in);
//			else
//				board = hungryMotion(board, in, buf.size());
//		}
//		catch (MotionException e) {
//			if (e.sad_print)
//				printBoard(board);
//			std::cout << "Aborted on the faulty input: " << e.str << ".\n";
//			complete = false;
//		}
//	} while (!complete);
//	return board;
//}
//
//BoardState playAutomatic(BoardState initial) {
//	std::cout << "Waiting till the move is computed... " << std::flush;
//	BoardState::apply(initial, minimax(initial));
//	std::cout << "The computer has moved.\n";
//	return initial;
//}
//
//using PlayerFunction = BoardState(*)(BoardState);
//
//void setPlayers(PlayerFunction players[2]) {
//	Role human;
//	std::cout << "This is a shashki playing session.\n";
//	do {
//		std::string w, buffer;
//		std::cout << "Please select either 'black' or 'white' as your color: ";
//		std::getline(std::cin, buffer);
//		std::istringstream in(buffer);
//		in >> w;
//		if (w == "black")
//			human = Role::Black;
//		if (w == "white")
//			human = Role::White;
//	} while (human == Role::None);
//	players[human] = playHuman;
//	players[human.opposite()] = playAutomatic;
//}
//
//
