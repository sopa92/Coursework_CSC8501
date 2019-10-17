//Course:		CSC8501
//Project:		Coursework part1
//Author:		Sofia Papadopoulou - 190338075
//Acad. Year:	2019-2020


#include "Puzzle.h"

#include <string>
#include <fstream>

using namespace std;

int GetInputNumber();
int GetInputNumber(Puzzle* puzzleArr);
bool isExistingNumber(int givenInput, Puzzle* puzzleArr);
void SettingValuesManually(Puzzle* puzzle, int rows, int cols);
void SettingValuesAuto(Puzzle* puzzle, int rows, int cols);
void CreatePuzzleConfigurationsRandomly();
void StorePuzzleInFile(Puzzle* puzzle);

int main()
{
	Puzzle mainPuzzle;
	int rows = (&mainPuzzle)->get_hor_size();
	int columns = (&mainPuzzle)->get_vert_size();
	//SettingValuesManually(&mainPuzzle, rows, columns);
	SettingValuesAuto(&mainPuzzle, rows, columns);
	cout << mainPuzzle << endl ;

	CreatePuzzleConfigurationsRandomly();
	return 0;
}

void SettingValuesManually(Puzzle* puzzle, int rows, int cols) {
	cout << "Please give me 15 numbers between 1 and 20." << endl;
	cout << "| 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 |" << endl;
	cout << "----------------------------------------------------------------------------" << endl;
	string printedArray = "| __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ |";
	string unfilledGap = "__";
	cout << printedArray << endl;	
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == rows - 1 && j == cols - 1) {
				puzzle->set_element(i,j,0);
				break;
			}
			puzzle->set_element(i, j, GetInputNumber(puzzle));
			string newElement = std::to_string(puzzle->get_element(i, j));
			printedArray.replace(printedArray.find(unfilledGap), unfilledGap.length(), newElement);
			system("cls");
			cout << printedArray << endl;
		}
	}
	cout << "Thank you! The 15-puzzle is now ready!" << endl;
}

void SettingValuesAuto(Puzzle* puzzle, int rows, int cols) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == rows - 1 && j == cols - 1) {
				puzzle->set_element(i, j, 0);
				break;
			}
			int rndNumber = rand() % 20;
			while (isExistingNumber(rndNumber, puzzle)) {
				rndNumber = rand() % 20;
			}
			puzzle->set_element(i, j, rndNumber);
		}
	}
}

void CreatePuzzleConfigurationsRandomly() {
	cout << "How many 15-puzzle configurations do you want me to create?" << endl;
	int inputAmount = GetInputNumber();
	ofstream my15file;
	if (!my15file.is_open())
	{
		my15file.open("15-File.txt");
	}
	my15file << inputAmount << endl;
	for (int i = 0; i < inputAmount; ++i) {
		Puzzle randPuzzle;
		SettingValuesAuto(&randPuzzle, randPuzzle.get_hor_size(), randPuzzle.get_vert_size());
		my15file << randPuzzle << endl;
	}
	my15file.close();
}

int GetInputNumber() {
	int givenInput = 0;
	cout << endl << "Type a number and press Enter:" << endl;
	cin >> givenInput;
	while (true) {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "This is a character. Please type a number." << endl;
			cin >> givenInput;
		}
		else {
			break;
		}
	}
	return givenInput;
}

int GetInputNumber(Puzzle* puzzleArr) {
	int givenInput = 0;
	cout << endl << "Type your chosen number and press Enter:" << endl;
	cin >> givenInput;
	while (true) {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "This is a character. Please type a number between 1 and 20." << endl;
			cin >> givenInput;
		}
		else if (givenInput < 1 || givenInput >20) {
			cout << "You should type a number between 1 and 20." << endl;
			cin >> givenInput;
		}
		else if (isExistingNumber(givenInput, puzzleArr)) {
			cout << "You must not choose an existing number. \nPlease type another number between 1 and 20." << endl;
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