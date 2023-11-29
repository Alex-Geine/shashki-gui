#include "pch.h"
#include "Game.h"


double** GetBoardMas(BoardState board) {
	//массив значений на доске
	double** res = new double* [8];
	for (int i = 0; i < 8; i++)
		res[i] = new double[8];

	Position p = board.position();


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Cell cell(i, j);
			res[i][j] = 0;
			if (cell.valid()) {
				if (p.ghost(cell))
					res[i][j] = 0;		//сьеденная шашка
				else if (p.color(cell) == Role::None)
					res[i][j] = 0;		//пустая шашка
				else {


					res[i][j] = p.color(cell) == Role::White ? 1 : -1;
					if (p.king(cell))
						res[i][j] *= 2;			//если дамка

				}
			}

		}

	}

	return res;
}

void printBoard(BoardState board) {
	Position p = board.position();
	std::cout << "\n\n      *********************************\n";
	for (int r = 7; r >= 0; --r) {
		unsigned char n = r + '1';
		std::cout << "    " << n << " *";
		for (int c = 0; c <= 7; ++c) {
			Cell cell(c, r);
			if (cell.valid()) {
				if (p.ghost(cell))
					std::cout << "***";
				else if (p.color(cell) == Role::None)
					std::cout << "///";
				else {
					char buffer[4];
					buffer[3] = '\0';
					for (int i = 0; i < 3; ++i)
						buffer[i] = p.color(cell) == Role::White ? 'w' : 'b';
					if (p.king(cell))
						buffer[1] = 'k';
					if (board.color() == p.color(cell))
						for (int i = 0; i < 3; ++i)
							buffer[i] += ('A' - 'a');
					std::cout << buffer;
				}
			}
			else
				std::cout << "   ";
			if (c != 7)
				std::cout << '|';
		}
		std::cout << "*\n";
		if (r != 0)
			std::cout << "      *---+---+---+---+---+---+---+---*\n";
	}
	std::cout << "      *********************************\n";
	std::cout << "        A   B   C   D   E   F   G   H  \n\n";
}

Direction parseDirection(std::string word, Role color) {
	if (color == Role::White) {
		if (word == "left") return Direction::LeftForward;
		if (word == "right") return Direction::RightForward;
		if (word == "left-back") return Direction::LeftBackward;
		if (word == "right-back") return Direction::RightBackward;
	}
	if (color == Role::Black) {
		if (word == "left") return Direction::LeftBackward;
		if (word == "right") return Direction::RightBackward;
		if (word == "left-back") return Direction::LeftForward;
		if (word == "right-back") return Direction::RightForward;
	}
	return Direction::None;
}

int parseInteger(std::string word) {
	int n = 0;
	for (char c : word) {
		if (c >= '0' && c <= '9')
			n = (c - '0') + n * 10;
		else
			return -1;
	}
	return n;
}

//ход со взятием шашек противника
BoardState hungryMotion(BoardState initial, std::istringstream& in, int bufsize) {
	std::string buf;
	bool sad_print = false;
	while (true) {
		std::string w;
		in >> w;
		Direction d = parseDirection(w, initial.color());	//направление
		bool king = initial.position().king(initial.place());	//дамка
		if (!initial.control(initial.place().neighbour(d)))	//ходит в выбранном нарпавлении
			throw MotionException("wrong direction (in hungry)", sad_print);
		while (!initial.capture())	//посмотри, что делает
			initial.control(initial.place().neighbour(d));
		int count = 1;	//делает минимум один ход, если обычная пешка
		if (king) {		//парсит на сколько клеток ходит дамка
			in >> w;
			count = parseInteger(w);
			if (count <= 0)
				throw MotionException("no motion count (in hungry)");
		}
		while (count--) {	//делает ходы
			if (!initial.control(initial.place().neighbour(d)))
				throw MotionException("the motion count is too high (in hungry)");
		}
		if (initial.control(Cell()))	//если все ок, то возвращает доску
			return initial;
		int g = in.tellg();				//если нет, то подготавливает к следующему ходу
		if (g == bufsize || g < 0) {
			sad_print = true;
			printBoard(initial);
			std::cout << "then: ";
			std::getline(std::cin, buf);
			in.str(buf);
			in.clear();
			bufsize = buf.size();
		}
	}
}

//ход без взятия шашек противника
BoardState quietMotion(BoardState initial, std::istringstream& in) {
	std::string w;
	in >> w;
	Direction d = parseDirection(w, initial.color());	//парсит направление хода
	bool king = initial.position().king(initial.place());	//проверяет, является ли шашка дамкой
	if (!initial.control(initial.place().neighbour(d)))		//проверяет, можно ли сходить на соседнюю клетку и ходит, если можно
		throw MotionException("wrong direction (in quiet)");
	if (king) {		
		in >> w;
		int count = parseInteger(w);	// смторит на сколько дамкой сходить
		if (count <= 0)
			throw MotionException("no motion count (in quiet)");
		while (--count) {		//в этом цикле ходит по одной клетке пока не дойдет илил обосрется
			if (!initial.control(initial.place().neighbour(d)))
				throw MotionException("the motion count is too high (in quiet)");
		}
	}
	initial.control(Cell());	//эта штука хз что делает
	return initial;
}

BoardState playHuman(BoardState initial, std::string Step) {
	if (initial.lost() || !initial.finished())
		return BoardState();
	
	BoardState board;
	//проверяется надо ли хавать
	bool quiet = initial.quiet();
	bool complete;
	
	do {
		complete = true;
		try {
			std::cout << "Your move: ";
			std::string buf;
			std::getline(std::cin, buf);
			std::istringstream in(buf);
			std::string w;
			in >> w;
			//проверяется выход из игры
			if (w == "quit")
				return BoardState();
			
			//получаем номер ячейки
			Cell cell = Cell::fromString(w);
			board = initial;
			
			//если не правильная ячейка
			if (!board.control(cell))
				throw MotionException("wrong cell");
			
			//если хавать не надо
			if (quiet)
				board = quietMotion(board, in);
			
			//если хавать надо
			else
				board = hungryMotion(board, in, buf.size());
		
		}
		catch (MotionException e) {
			return initial;
		}

	} while (!complete);
	return board;
}

BoardState playAutomatic(BoardState initial) {
	std::cout << "Waiting till the move is computed... " << std::flush;
	BoardState::apply(initial, minimax(initial));
	std::cout << "The computer has moved.\n";
	return initial;
}