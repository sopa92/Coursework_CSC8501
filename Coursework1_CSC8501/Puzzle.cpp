#include "Puzzle.h"

Puzzle::Puzzle(int x, int y): hor_size(x), vert_size(y)
{
	puzzleArr = new int* [hor_size];
	for (int i = 0; i < hor_size; ++i) {
		puzzleArr[i] = new int[vert_size];
		memset(puzzleArr[i], 0, (vert_size * sizeof(int)));
	}
}

Puzzle::Puzzle(Puzzle* puz): hor_size(puz->Get_hor_size()), vert_size(puz->Get_vert_size())
{
	puzzleArr = new int* [hor_size];
	for (int i = 0; i < hor_size; ++i) {
		puzzleArr[i] = new int[vert_size];
		memset(puzzleArr[i], 0, (vert_size * sizeof(int)));
	}

	for (int i = 0; i < hor_size; ++i) {
		for (int j = 0; j < vert_size; ++j) {
			this->Set_element(i, j, puz->Get_element(i, j));
		}
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
	for (int i = 0; i < puzzle.Get_hor_size(); ++i) {
		for (int j = 0; j < puzzle.Get_vert_size(); ++j) {
			int value = puzzle.Get_element(i, j);
			if(value!=0)
			ostr << value << " ";
			else 
				ostr << " ";
		}
		ostr << std::endl;
	}
	return ostr;
}

std::string Puzzle::ToString()
{
	std::string puzzleArrString;
	for (int i = 0; i < (*this).Get_hor_size(); ++i) {
		for (int j = 0; j < (*this).Get_vert_size(); ++j) {
			puzzleArrString += std::to_string((*this).Get_element(i, j));
			puzzleArrString += " ";
		}
	}
	return puzzleArrString;
}

void Puzzle::GetAvailableSwaps(std::vector<char>& availSwaps, int rows, int cols, int zero_x, int zero_y ){
	if (zero_x < rows - 1)
		availSwaps.push_back('D');
	if (zero_x > 0)
		availSwaps.push_back('U');
	if (zero_y < cols - 1)
		availSwaps.push_back('R');
	if (zero_y > 0)
		availSwaps.push_back('L');
}

bool Puzzle::MoveRandomlyAround(char direction, int zero_x, int zero_y, Puzzle* puzzle, int prev_zero_x, int prev_zero_y, int attempts) {
	
	switch (direction) {
	case 'U':
		if (prev_zero_x == zero_x - 1 && prev_zero_y == zero_y && attempts>0) {
			return false;
		}
		swap(zero_x, zero_y, puzzle, zero_x - 1, zero_y);
		break;
	case 'D':
		if (prev_zero_x == zero_x + 1 && prev_zero_y == zero_y && attempts > 0) {
			return false;
		}
		swap(zero_x, zero_y, puzzle, zero_x + 1, zero_y);
		break;
	case 'L':
		if (prev_zero_x == zero_x && prev_zero_y == zero_y - 1 && attempts > 0) {
			return false;
		}
		swap(zero_x, zero_y, puzzle, zero_x, zero_y - 1);
		break;
	case 'R':
		if (prev_zero_x == zero_x && prev_zero_y == zero_y + 1 && attempts > 0) {
			return false;
		}
		swap(zero_x, zero_y, puzzle, zero_x, zero_y + 1);
		break;
	}
	++attempts;
	return true;
}

void Puzzle::moveRight(int x, int y, Puzzle& puzzle) {
	swap(x, y, &puzzle, x, y + 1);
}

void Puzzle::moveLeft(int x, int y, Puzzle& puzzle) {
	swap(x, y, &puzzle, x, y - 1);
}

void Puzzle::moveUp(int x, int y, Puzzle& puzzle) {
	swap(x, y, &puzzle, x - 1, y);
}

void Puzzle::moveDown(int x, int y, Puzzle& puzzle) {
	swap(x, y, &puzzle, x + 1, y);
}

void Puzzle::swap(int x, int y, Puzzle* puzzle, int new_x, int new_y) {
	puzzle->Set_element(x, y, puzzle->Get_element(new_x, new_y));
	puzzle->Set_element(new_x, new_y, 0);
	puzzle->Set_zero_position(new_x, new_y);
}
