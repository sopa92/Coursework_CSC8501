//Course:		CSC8501
//Project:		Coursework part1
//Author:		Sofia Papadopoulou - 190338075
//Acad. Year:	2019-2020


#include "Puzzle.h"

#include <string>

using namespace std;


int GetInputNumber(Puzzle* puzzleArr);
bool isExistingNumber(int givenInput, Puzzle* puzzleArr);
void SettingValuesManually(Puzzle* puzzle);

int main()
{
	Puzzle mainPuzzle;
	SettingValuesManually(&mainPuzzle);

	cout << mainPuzzle;
	return 0;
}

void SettingValuesManually(Puzzle* puzzle) {
	cout << "Please give me 15 numbers between 1 and 20." << endl;
	cout << "| 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 |" << endl;
	cout << "----------------------------------------------------------------------------" << endl;
	string printedArray = "| __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ |";
	string unfilledGap = "__";
	cout << printedArray << endl;
	int rows = puzzle->get_hor_size();
	int cols = puzzle->get_vert_size();
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == rows - 1 && j == cols - 1) {
				puzzle->set_element(i,j,0);
				break;
			}
			puzzle->set_element(i, j, GetInputNumber(puzzle));
			string newElement = std::to_string(puzzle->get_element(i, j));
			printedArray.replace(printedArray.find(unfilledGap), unfilledGap.length(), newElement);
			cout << printedArray << endl;
		}
	}
	cout << "Thank you! The 15-puzzle is now ready!" << endl;
}


int GetInputNumber(Puzzle* puzzleArr) {
	int givenInput = 0;
	while (true) {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "This is a character. Please choose a number between 1 and 20." << endl;
			cin >> givenInput;
		}
		else if (givenInput < 1 || givenInput >20) {
			cout << "You should choose a number between 1 and 20." << endl;
			cin >> givenInput;
		}
		else if (isExistingNumber(givenInput, puzzleArr)) {
			cout << "You must not choose an existing number. \n Please choose another number between 1 and 20." << endl;
			cin >> givenInput;
		}
		else {
			break;
		}
	}

	return givenInput;
}

bool isExistingNumber(int givenInput, Puzzle* puzzleArr) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (puzzleArr->get_element(i,j) == givenInput)
				return true;
		}
	}
	return false;
}