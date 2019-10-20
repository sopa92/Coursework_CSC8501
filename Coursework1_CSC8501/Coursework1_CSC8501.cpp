//Course:		CSC8501
//Project:		Coursework part1
//Author:		Sofia Papadopoulou - 190338075
//Acad. Year:	2019-2020


#include "Puzzle.h"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int GetInputNumber();
int GetInputNumber(Puzzle* puzzleArr);
bool isExistingNumber(int givenInput, Puzzle* puzzleArr);
void SettingValuesManually(Puzzle* puzzle);
void SettingValuesAuto(Puzzle* puzzle);
void CreatePuzzleConfigurationsRandomly();
void moveAndCalculateContinuous(Puzzle* puzzle);
int FindContinuousRows(Puzzle* puzzle, bool reversed);
int FindContinuousCols(Puzzle* puzzle, bool reversed);
bool startOverGame();
void ReadPuzzleFromFileAndMoveAround();
void MoveAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence);
void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle);
bool turnHasCompleted(Puzzle* puzzle);
void readPuzzleArrayFromFile(Puzzle* puz, vector<string>& fullRows, string fullRowString, int& fileRow, int& i);
int rows, columns;

int main()
{
	bool startOver = true;
	while (startOver) {
		Puzzle mainPuzzle;
		rows = (&mainPuzzle)->Get_hor_size();
		columns = (&mainPuzzle)->Get_vert_size();
		//SettingValuesManually(&mainPuzzle);
		//SettingValuesAuto(&mainPuzzle);
		//cout << mainPuzzle << endl;

		CreatePuzzleConfigurationsRandomly();

		ReadPuzzleFromFileAndMoveAround();
		//startMovingRandomly(&mainPuzzle);

		startOver = startOverGame();
	}
	return 0;
}

void ReadPuzzleFromFileAndMoveAround() {
	try {
		ifstream my15file;
		my15file.open("15-File.txt");
		string fullRowString;
		istringstream iss(fullRowString);
		vector<string> fullRows;
		Puzzle puz;
		int i = 0;
		int fileRow = 0;
		while (getline(my15file, fullRowString)) {
			if (i >= rows) {
				moveAndCalculateContinuous(&puz);
				Puzzle puz;
				i = 0;
			}
			if (fullRowString.size() > 7 && i < rows) {
				readPuzzleArrayFromFile(&puz, fullRows, fullRowString, fileRow, i);
			}
		}
		my15file.close();
	}
	catch(exception){
		cout << "In order to use a file, I have to create it first." << endl;
	}
}

void readPuzzleArrayFromFile(Puzzle* puz, vector<string>& fullRows, string fullRowString, int& fileRow, int& i) {
	fullRows.push_back(fullRowString);
	stringstream ssin(fullRows[fileRow]);
	for (int j = 0; j < columns; ++j) {
		if (i == rows - 1 && j == columns - 1) {
			puz->Set_element(i, j, 0);
			puz->Set_zero_position(i, j);
			break;
		}
		if (ssin.good()) {
			int intValue;
			ssin >> intValue;
			if (!fullRows.empty())
				puz->Set_element(i, j, intValue);
		}
	}
	++i;
	++fileRow;
}

void moveAndCalculateContinuous(Puzzle* puzzle) {
	cout << endl << *puzzle;
	int countMoves = 1;
	int prevPositionX = rows - 1;
	int prevPositionY = columns - 1;
	int continuousRowsPerTurn = 0, continuousColsPerTurn = 0, revContinuousRowsPerTurn = 0, revContinuousColsPerTurn = 0;
	int continuousRowsTotal = 0, continuousColsTotal = 0, revContinuousRowsTotal = 0, revContinuousColsTotal = 0;
	vector<string> movesSequences;
	int existingPath = 0;
	while (existingPath<1000) {
		string moveSequence;
		continuousRowsPerTurn = continuousColsPerTurn = revContinuousRowsPerTurn = revContinuousColsPerTurn = 0;
		do {
			MoveAround(puzzle, prevPositionX, prevPositionY, moveSequence);
			FindContinuousElements(continuousRowsPerTurn, revContinuousRowsPerTurn, continuousColsPerTurn, revContinuousColsPerTurn, puzzle);
			if (turnHasCompleted(puzzle)) {
				if (find(movesSequences.begin(), movesSequences.end(), moveSequence) == movesSequences.end())
				{
					movesSequences.push_back(moveSequence);
				}
				else {
					existingPath++;
					continuousRowsPerTurn = continuousColsPerTurn = revContinuousRowsPerTurn = revContinuousColsPerTurn = 0;
					break;
				}
			}			
		} while (!turnHasCompleted(puzzle));
		continuousRowsTotal += continuousRowsPerTurn;
		continuousColsTotal += continuousColsPerTurn;
		revContinuousRowsTotal += revContinuousRowsPerTurn;
		revContinuousColsTotal += revContinuousColsPerTurn;
	}
	cout << "row: " << continuousRowsTotal << endl;
	cout << "column: " << continuousColsTotal << endl;
	cout << "reverse row: " << revContinuousRowsTotal << endl;
	cout << "reverse column: " << revContinuousColsTotal << endl;
}

bool turnHasCompleted(Puzzle* puzzle) {
	return (puzzle->Get_zero_x() == rows - 1 && puzzle->Get_zero_y() == columns - 1);
}

void MoveAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence) {
	vector<char> availSwaps;
	bool moved = false;
	int zero_x = puzzle->Get_zero_x();
	int zero_y = puzzle->Get_zero_y();
	puzzle->GetAvailableSwaps(availSwaps, rows, columns, zero_x, zero_y);
	int attempts = 1;
	while (!moved) {
		int randValue = rand() % availSwaps.size();
		char direction = availSwaps[randValue];
		moved = puzzle->MoveRandomlyAround(direction, zero_x, zero_y, puzzle, prevPositionX, prevPositionY, attempts);
		if (!moved)
			availSwaps.erase(availSwaps.begin() + (randValue));
		else {
			moveSequence += direction;
			prevPositionX = zero_x;
			prevPositionY = zero_y;
		}
	}
	availSwaps.clear();
}

void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle) {
	contRows += FindContinuousRows(puzzle, false);
	revContRows += FindContinuousRows(puzzle, true);
	contCols += FindContinuousCols(puzzle, false);
	revContCols += FindContinuousCols(puzzle, true);
}

int FindContinuousRows(Puzzle* puzzle, bool reversed) {
	int continuousRows = 0;
	bool isContinuous = true;
	for (int i = 0; i < rows; ++i) {
		isContinuous = true;
		if (puzzle->Get_element(i, columns - 1) == 0) {
			isContinuous = false;
			continue;
		}
		for (int j = 0; j < columns - 1; ++j) {
			int firstElement = puzzle->Get_element(i, j);
			int nextElement = puzzle->Get_element(i, j + 1);
			if (firstElement == 0) {
				isContinuous = false;
				break;
			}
			if (reversed) {
				if (nextElement - firstElement != 1) {
					isContinuous = false;
				}
			}
			else {
				if (firstElement - nextElement != 1) {
					isContinuous = false;
				}
			}
		}
		if (isContinuous) {
			++continuousRows; 
		}
	}
	return continuousRows;
}

int FindContinuousCols(Puzzle* puzzle, bool reversed) {
	int continuousCols = 0;
	bool isContinuous = true;
	for (int j = 0; j < columns; ++j) {
		isContinuous = true;
		if (puzzle->Get_element(rows - 1, j) == 0) {
			isContinuous = false;
			continue;
		}
		for (int i = 0; i < rows - 1; ++i) {
			int firstElement = puzzle->Get_element(i, j);
			int nextElement = puzzle->Get_element(i + 1, j);
			if (firstElement == 0) {
				isContinuous = false;
				break;
			}
			if (reversed) {
				if (nextElement - firstElement != 1) {
					isContinuous = false;
				}
			}
			else {
				if (firstElement - nextElement != 1) {
					isContinuous = false;
				}
			}
		}
		if (isContinuous) {
			++continuousCols;
			//cout << "Col: " << puzzle->Get_element(0, j) << " < " << puzzle->Get_element(1, j) << " < " << puzzle->Get_element(2, j) << " < " << puzzle->Get_element(3, j) << endl;
			/*cout << "there are continuous cols" << endl;
			cout << *puzzle;*/
		}
	}
	return continuousCols;
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

void SettingValuesManually(Puzzle* puzzle) {
	cout << "Please give me 15 numbers between 1 and 20." << endl;
	cout << "| 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 |" << endl;
	cout << "----------------------------------------------------------------------------" << endl;
	string printedArray = "| __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ | __ |";
	string unfilledGap = "__";
	cout << printedArray << endl;	
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (i == rows - 1 && j == columns - 1) {
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

void SettingValuesAuto(Puzzle* puzzle) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (i == rows - 1 && j == columns - 1) {
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
		SettingValuesAuto(&randPuzzle);
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