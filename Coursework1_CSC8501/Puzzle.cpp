#include "Puzzle.h"

Puzzle::Puzzle(int x, int y): hor_size(x), vert_size(y)
{
	puzzleArr = new int* [hor_size];
	for (int i = 0; i < hor_size; ++i) {
		puzzleArr[i] = new int[vert_size];
		memset(puzzleArr[i], 0, (vert_size * sizeof(int)));
	}
}

Puzzle::~Puzzle()
{
	for (int i = 0; i < hor_size; ++i) {
		delete[] puzzleArr[i];
	}
	delete[] puzzleArr;
}

