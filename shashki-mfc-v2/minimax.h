#pragma once
#ifndef MINIMAX_H
#define MINIMAX_H

#include "board_state.h"
#include <vector>

std::vector<Cell> minimax(BoardState board);
std::vector<std::vector<Cell>> explore(const BoardState& board);

#endif
