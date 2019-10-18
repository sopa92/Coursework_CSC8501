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
void startMovingRandomly(Puzzle* puzzle, int rows, int cols);
int FindContinuousRows(Puzzle* puzzle, int rows, int cols);
int FindReversedContinuousRows(Puzzle* puzzle, int rows, int cols);
int FindReversedContinuousCols(Puzzle* puzzle, int rows, int cols);
int FindContinuousCols(Puzzle* puzzle, int rows, int cols);
bool startOverGame();

int main()
{
	bool startOver = true;
	while (startOver) {
		Puzzle mainPuzzle;
		int rows = (&mainPuzzle)->Get_hor_size();
		int columns = (&mainPuzzle)->Get_vert_size();
		SettingValuesManually(&mainPuzzle, rows, columns);
		//SettingValuesAuto(&mainPuzzle, rows, columns);
		cout << mainPuzzle << endl;

		//CreatePuzzleConfigurationsRandomly();

		
		startMovingRandomly(&mainPuzzle, rows, columns);

		startOver = startOverGame();
	}
	return 0;
}


void startMovingRandomly(Puzzle* puzzle, int rows, int cols) {
	vector<char> availSwaps;
	int countMoves = 1;
	int prevPositionX = rows - 1;
	int prevPositionY = cols - 1;
	int continuousRows = 0, continuousCols = 0, revContinuousRows = 0, revContinuousCols = 0;
	do {
		bool moved = false;
		int zero_x = puzzle->Get_zero_x();
		int zero_y = puzzle->Get_zero_y();
		puzzle->GetAvailableSwaps(availSwaps, rows, cols, zero_x, zero_y);
		int attempts = 0;
		while (!moved) {
			int randValue = rand() % availSwaps.size();
			char direction = availSwaps[randValue];
			moved = puzzle->MoveRandomlyAround(direction, zero_x, zero_y, puzzle, prevPositionX, prevPositionY, attempts);
			if(!moved)
				availSwaps.erase(availSwaps.begin() + (randValue));
			else {
				prevPositionX = zero_x;
				prevPositionY = zero_y;
			}
		}
		availSwaps.clear();
		countMoves++;
		cout << endl << *puzzle;
		continuousRows += FindContinuousRows(puzzle, rows, cols);
		revContinuousRows += FindReversedContinuousRows(puzzle, rows, cols);
		continuousCols += FindContinuousCols(puzzle, rows, cols);
		revContinuousCols += FindReversedContinuousCols(puzzle, rows, cols);
	} while (puzzle->Get_zero_x() != rows - 1 || puzzle->Get_zero_y() != cols - 1);
	cout << endl << countMoves << endl << endl;
	cout << "row: " << continuousRows << endl;
	cout << "column: " << continuousCols << endl;
	cout << "reverse row: " << revContinuousRows << endl;
	cout << "reverse column: " << revContinuousCols << endl;
}

int FindContinuousRows(Puzzle* puzzle, int rows, int cols) {
	int continuousRows = 0;
	bool isContinuous = true;
	for (int i = 0; i < rows; ++i) {
		isContinuous = true;
		for (int j = 0; j < cols-1; ++j) {
			if (puzzle->Get_element(i, j) + 1 != puzzle->Get_element(i, j + 1)) {
				isContinuous = false;
			}
		}
		if (isContinuous) {
			++continuousRows;
		}
	}
	return continuousRows;
}

int FindReversedContinuousRows(Puzzle* puzzle, int rows, int cols) {
	int revContRows = 0;
	bool isContinuous = true;
	for (int i = 0; i < rows; ++i) {
		isContinuous = true;
		for (int j = 0; j < cols - 1; ++j) {
			if (puzzle->Get_element(i, j) - 1 != puzzle->Get_element(i, j + 1)) {
				isContinuous = false;
			}
		}
		if (isContinuous) {
			++revContRows;
		}
	}
	return revContRows;
}

int FindContinuousCols(Puzzle* puzzle, int rows, int cols) {
	int continuousCols = 0;
	bool isContinuous = true;
	for (int j = 0; j < cols; ++j) {
		isContinuous = true;
		for (int i = 0; i < rows - 1; ++i) {
			if (puzzle->Get_element(i, j) + 1 != puzzle->Get_element(i + 1, j)) {
				isContinuous = false;
			}
		}
		if (isContinuous) {
			++continuousCols;
		}
	}
	return continuousCols;
}

int FindReversedContinuousCols(Puzzle* puzzle, int rows, int cols) {
	int revContCols = 0;
	bool isContinuous = true;
	for (int j = 0; j < cols; ++j) {
		isContinuous = true;
		for (int i = 0; i < rows - 1; ++i) {
			if (puzzle->Get_element(i, j) - 1 != puzzle->Get_element(i + 1, j)) {
				isContinuous = false;
			}
		}
		if (isContinuous) {
			++revContCols;
		}
	}
	return revContCols;
}

bool startOverGame() {
	cout << "Do you want to start over? (Y/N)" << endl;
	char answer;
	cin >> answer;
	answer = tolower(answer);
	while (true) {
		if (cin.fail() || (answer != 'y' && answer != 'n')) {
			cout << "Invalid input. Please answer Y (for yes) or N (for No): " << endl;
			cin >> answer;
		}
		else {
			break;
		}
	}
	return answer == 'y' ? true: false;
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
				puzzle->Set_element(i,j,0);
				puzzle->Set_zero_position(i, j);
				break;
			}
			puzzle->Set_element(i, j, GetInputNumber(puzzle));
			string newElement = std::to_string(puzzle->Get_element(i, j));
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
				puzzle->Set_element(i, j, 0);
				puzzle->Set_zero_position(i, j);
				break;
			}
			int rndNumber = rand() % 20;
			while (isExistingNumber(rndNumber, puzzle)) {
				rndNumber = rand() % 20;
			}
			puzzle->Set_element(i, j, rndNumber);
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
		SettingValuesAuto(&randPuzzle, randPuzzle.Get_hor_size(), randPuzzle.Get_vert_size());
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
			cout << "Invalid input. Please type a number." << endl;
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
			cout << "Invalid input. Please type a number between 1 and 20." << endl;
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
			if (puzzleArr->Get_element(i,j) == givenInput)
				return true;
		}
	}
	return false;
}