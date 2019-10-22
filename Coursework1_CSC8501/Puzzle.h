#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <string>

class Puzzle {
public:
	static const int default_rows = 3;
	static const int default_cols = 3;
	Puzzle(int x = default_rows, int y = default_cols);
	Puzzle(Puzzle* puz);
	~Puzzle();
	void Set_element(int row, int col, int new_value) { puzzleArr[row][col] = new_value; }
	int Get_element(int row, int col) const { return puzzleArr[row][col]; }
	int Get_hor_size() const { return hor_size; }
	int Get_vert_size() const { return vert_size; }
	friend std::ostream& operator << (std::ostream& ostr, const Puzzle& puzzle);
	void GetAvailableSwaps(std::vector<char>& availSwaps, int rows, int cols, int zero_x, int zero_y);
	bool MoveRandomlyAround(char direction, int zero_x, int zero_y, Puzzle* puzzle, int prev_zero_x, int prev_zero_y, int attempts);
	int Get_zero_x() const { return zero_pos_x; }
	int Get_zero_y() const { return zero_pos_y; }
	void Set_zero_position(int new_zero_x, int new_zero_y) { zero_pos_x = new_zero_x; zero_pos_y = new_zero_y; }
protected:
	int** puzzleArr;
	int hor_size;
	int vert_size;
	int zero_pos_x;
	int zero_pos_y;
	void moveRight(int x, int y, Puzzle& puzzle);
	void moveLeft(int x, int y, Puzzle& puzzle);
	void moveUp(int x, int y, Puzzle& puzzle);
	void moveDown(int x, int y, Puzzle& puzzle);
	void swap(int x, int y, Puzzle* puzzle, int new_x, int new_y);
};