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

std::ostream& operator<<(std::ostream& ostr, const Puzzle& puzzle)
{
	for (int i = 0; i < puzzle.get_hor_size(); ++i) {
		for (int j = 0; j < puzzle.get_vert_size(); ++j) {
			ostr << puzzle.get_element(i, j) << "\t";
		}
		ostr << std::endl;
	}
	return ostr;
}
