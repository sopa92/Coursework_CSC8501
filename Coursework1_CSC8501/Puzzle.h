#pragma once

#include <iostream>

class Puzzle {
public:
	static const int default_rows = 4;
	static const int default_cols = 4;
	Puzzle(int x = default_rows, int y = default_cols);
	~Puzzle();
	void set_element(int row, int col, int new_value) { puzzleArr[row][col] = new_value; }
	int get_element(int row, int col) const { return puzzleArr[row][col]; }
	int get_hor_size() const { return hor_size; }
	int get_vert_size() const { return vert_size; }
protected:
	int** puzzleArr;
	int hor_size;
	int vert_size;
};