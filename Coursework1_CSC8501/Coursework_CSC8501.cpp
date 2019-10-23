//Course:		CSC8501
//Project:		Coursework part1
//Author:		Sofia Papadopoulou - 190338075
//Acad. Year:	2019-2020


#include "Puzzle.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;
void PrintMenu();
int GetInputNumber(int min, int max);
int GetInputNumber(Puzzle* puzzleArr);
bool isExistingNumber(int givenInput, Puzzle* puzzleArr);
void SettingValuesManually(Puzzle* puzzle);
void SettingValuesAuto(Puzzle* puzzle);
void CreatePuzzleConfigurationsRandomly();
string moveAndCalculateContinuous(Puzzle* puzzle, int count, char printAction);
int FindContinuousRows(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements);
int FindContinuousCols(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements);
bool startOverGame();
void ReadPuzzleFromFileAndMoveAround();
void MoveAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence);
void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle, vector<string>& calculatedElements);
bool turnHasCompleted(Puzzle* puzzle);
void readPuzzleArrayFromFile(Puzzle* puz, vector<string>& fullRows, string fullRowString, int& fileRow, int& i);
void PrintContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal);
int AskUserForContinuousElements();
void FindNPartialContinuousElements(Puzzle* mainPuzzle, int N);
char GetInputOption(char optionA, char optionB);
int rows, columns;

int main()
{	
	bool startOver = true;

	while (startOver) {

		PrintMenu();

		int response = GetInputNumber(1, 3);

		Puzzle mainPuzzle;
		rows = (&mainPuzzle)->Get_hor_size();
		columns = (&mainPuzzle)->Get_vert_size();

		if (response == 1) {
			SettingValuesManually(&mainPuzzle);
			//SettingValuesAuto(&mainPuzzle);
			cout << mainPuzzle << endl;
			int N = AskUserForContinuousElements();
			FindNPartialContinuousElements(&mainPuzzle, N);
		}
		else if (response == 2) {
			CreatePuzzleConfigurationsRandomly();
		}
		else
			ReadPuzzleFromFileAndMoveAround();
		
		//startMovingRandomly(&mainPuzzle);

		startOver = startOverGame();
	}
	return 0;
}

int AskUserForContinuousElements() {
	cout << "Do you want me to calculate how many N-partial continuous rows, columns, \nreverse rows and reverse columns exist in this configuration? (Y/N)" << endl;
	char answer = GetInputOption('Y', 'N');
	if (answer == 'N') {
		return 0;
	}
	cout << "What will be the N?" << endl;
	int N = GetInputNumber(2, 4);
	return N;
}
void FindNPartialContinuousElements(Puzzle* mainPuzzle, int N) {
	
	int twoPartialRows = 0, twoPartialCols = 0, twoPartialRevRows = 0, twoPartialRevCols = 0;
	vector<string> calculatedElements;
	int pos = 0;
	for (int i = 0; i <= rows - N; ++i) {
		for (int j = 0; j <= columns - N; ++j) {
			Puzzle* miniPuzzle = new Puzzle(N, N);
			for (int posX = 0; posX < N; ++posX) {
				for (int posY = 0; posY < N; ++posY) {
					miniPuzzle->Set_element(posX, posY, mainPuzzle->Get_element(i + posX, j + posY));
				}
			}
			FindContinuousElements(twoPartialRows, twoPartialRevRows, twoPartialCols, twoPartialRevCols, miniPuzzle, calculatedElements);
			delete miniPuzzle;
			miniPuzzle = NULL;
		}
	}
	PrintContinuousElements(twoPartialRows, twoPartialCols, twoPartialRevRows, twoPartialRevCols);
}

void ReadPuzzleFromFileAndMoveAround() {
	try {
		cout << "Do you want me also to print the configurations, apart from exporting them in a file? (Y/N)" << endl;
		char printAction = GetInputOption('Y', 'N');
		ifstream my15file;
		my15file.open("15-File.txt");
		string fullRowString;
		istringstream iss(fullRowString);
		vector<string> fullRows;
		Puzzle puz;
		int i = 0;
		int fileRow = 0;
		int countPuzzles=0;
		int minRowSize = (rows * 2) - 2;
		string resultsForFile;
		while (getline(my15file, fullRowString)) {
			if (i >= rows) {
				++countPuzzles;;				
				resultsForFile += moveAndCalculateContinuous(&puz, countPuzzles, printAction);
				if(printAction=='n' and 0 == fileRow % 10)
					cout << ". ";
				Puzzle puz;
				i = 0;
			}
			if (fullRowString.size() > minRowSize && i < rows) {
				readPuzzleArrayFromFile(&puz, fullRows, fullRowString, fileRow, i);
			}
		}
		my15file.close();

		ofstream solutionFile;
		solutionFile.open("Solution-File.txt");
		solutionFile << resultsForFile;
		cout << endl << "Results are now written in the Solution-File.txt !" << endl;
		solutionFile.close();
	}
	catch(exception){
		cout << "In order to use a file, you have to create it first." << endl;
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

string moveAndCalculateContinuous(Puzzle* puzzle, int count, char printAction) {
	
	std::ostringstream resultsStrStream;
	resultsStrStream << count << "\n";
	resultsStrStream << *puzzle;
	if (printAction == 'Y') {
		cout << endl << *puzzle;
	}
	int prevPositionX = rows - 1;
	int prevPositionY = columns - 1;
	int continuousRowsPerTurn = 0, continuousColsPerTurn = 0, revContinuousRowsPerTurn = 0, revContinuousColsPerTurn = 0;
	int continuousRowsTotal = 0, continuousColsTotal = 0, revContinuousRowsTotal = 0, revContinuousColsTotal = 0;
	vector<string> movesSequences;
	int existingPath = 0;
	while (existingPath < 10) {
		string moveSequence;
		continuousRowsPerTurn = continuousColsPerTurn = revContinuousRowsPerTurn = revContinuousColsPerTurn = 0;
		do {
			MoveAround(puzzle, prevPositionX, prevPositionY, moveSequence);
			FindContinuousElements(continuousRowsPerTurn, revContinuousRowsPerTurn, continuousColsPerTurn, revContinuousColsPerTurn, puzzle, movesSequences);
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
	
	if (printAction == 'Y') {
		PrintContinuousElements(continuousRowsTotal, continuousColsTotal, revContinuousRowsTotal, revContinuousColsTotal);
	}
	resultsStrStream << "row: " << continuousRowsTotal << endl << "column: " << continuousColsTotal << endl;
	resultsStrStream << "reverse row: " << revContinuousRowsTotal << endl << "reverse column: " << revContinuousColsTotal << endl << endl;
	string results = resultsStrStream.str();
	return results;
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

bool startOverGame() {
	cout << "Do you want to start over? (Y/N)" << endl;
	char answer = GetInputOption('Y', 'N');
	if (answer == 'Y') {
		system("CLS");
		return true;
	}
	return false;
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
			int rndNumber = rand() % (rows * 5);
			while (isExistingNumber(rndNumber, puzzle)) {
				rndNumber = rand() % (rows * 5);
			}
			puzzle->Set_element(i, j, rndNumber);
		}
	}
}

void CreatePuzzleConfigurationsRandomly() {
	cout << "How many 15-puzzle configurations do you want me to create?" << endl;
	int inputAmount = GetInputNumber(0,0);
	cout << "Do you want me also to print the configurations, apart from exporting them in a file? (Y/N)" << endl;
	char printAction = GetInputOption('Y', 'N');
	ofstream my15file;
	if (!my15file.is_open())
	{
		my15file.open("15-File.txt");
	}
	my15file << inputAmount << endl;
	if (printAction == 'Y') {
		cout << inputAmount << endl;
	}
	for (int i = 0; i < inputAmount; ++i) {
		Puzzle randPuzzle;
		SettingValuesAuto(&randPuzzle);
		my15file << randPuzzle << endl;
		if (printAction == 'Y') {
			cout << randPuzzle << endl;
		}
		else if(inputAmount > 5000 and  0 == i % 100)
			cout << ". ";
	}
	cout << endl << "Generated puzzles are succesfully written in the 15-File.txt !" << endl;
	my15file.close();
}

char GetInputOption(char optionA, char optionB){
	char answer;
	cin >> answer;
	answer = tolower(answer);
	while (true) {
		if (cin.fail() || (answer != tolower(optionA) && answer != tolower(optionB))) {
			cout << "Invalid input. Please type " << optionA << " or " << optionB << ": " << endl;
			cin >> answer;
		}
		else {
			break;
		}
	}
	return toupper(answer);
}

int GetInputNumber(int min, int max) {
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
			if (min != max && (givenInput<min || givenInput>max)) {
				cout << "Invalid input. Please type a number between " << min << " and " << max << "." << endl;
				cin >> givenInput;
			}
			else {
				break;
			}
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
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (puzzleArr->Get_element(i,j) == givenInput)
				return true;
		}
	}
	return false;
}

void PrintMenu() {
	cout << endl;
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - +" << endl;
	cout << "| W | e | l | c | o | m | e |  | a | b | o | a | r | d |" << endl;
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - +" << endl;
	cout << endl;
	cout << "What do you wish to do? Choose a number from below." << endl;
	cout << " 1 - I want to create my own 15-puzzle." << endl;
	cout << " 2 - I want you to generate some 15-puzzles and write them in a file." << endl;
	cout << " 3 - I want to read 15-puzzles from a file and deduce continuous elements in all their turns." << endl;
}

void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle, vector<string>& calculatedElements) {
	contRows += FindContinuousRows(puzzle, false, calculatedElements);
	revContRows += FindContinuousRows(puzzle, true, calculatedElements);
	contCols += FindContinuousCols(puzzle, false, calculatedElements);
	revContCols += FindContinuousCols(puzzle, true, calculatedElements);
}

void PrintContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal) {
	cout << "row: " << contRowsTotal << endl;
	cout << "column: " << contColsTotal << endl;
	cout << "reverse row: " << revContRowsTotal << endl;
	cout << "reverse column: " << revContColsTotal << endl;
}

int FindContinuousRows(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements) {
	int rows = puzzle->Get_hor_size();
	int cols = puzzle->Get_vert_size();
	string checkedElements;
	int continuousRows = 0;
	bool isContinuous = true;
	for (int i = 0; i < rows; ++i) {
		isContinuous = true;
		if (puzzle->Get_element(i, cols - 1) == 0) {
			isContinuous = false;
			checkedElements.clear();
			continue;
		}
		for (int j = 0; j < cols - 1; ++j) {
			int firstElement = puzzle->Get_element(i, j);
			int nextElement = puzzle->Get_element(i, j + 1);
			if (checkedElements.find(to_string(firstElement)) == string::npos) {
				checkedElements += checkedElements.empty() ? to_string(firstElement) : "_" + to_string(firstElement);
			}
			checkedElements += "_" + to_string(nextElement);
			if (firstElement == 0) {
				isContinuous = false;
				checkedElements.clear();
				break;
			}
			if ((!reversed && (firstElement - nextElement != -1)) || (reversed && (firstElement - nextElement != 1))) {
				isContinuous = false;
				checkedElements.clear();
			}
		}
		if (isContinuous) {
			if (find(calculatedElements.begin(), calculatedElements.end(), checkedElements) == calculatedElements.end()) {
				++continuousRows;
				calculatedElements.push_back(checkedElements);
			}
		}
		checkedElements.clear();
	}
	return continuousRows;
}

int FindContinuousCols(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements) {
	int rows = puzzle->Get_hor_size();
	int cols = puzzle->Get_vert_size();
	string checkedElements;
	int continuousCols = 0;
	bool isContinuous = true;
	for (int j = 0; j < cols; ++j) {
		isContinuous = true;
		if (puzzle->Get_element(rows - 1, j) == 0) {
			isContinuous = false;
			checkedElements.clear();
			continue;
		}
		for (int i = 0; i < rows - 1; ++i) {
			int firstElement = puzzle->Get_element(i, j);
			int nextElement = puzzle->Get_element(i + 1, j);
			if (checkedElements.find(to_string(firstElement)) == string::npos) {
				checkedElements += checkedElements.empty() ? to_string(firstElement) : "_" + to_string(firstElement);
			}
			checkedElements += "_" + to_string(nextElement);
			if (firstElement == 0) {
				isContinuous = false;
				checkedElements.clear();
				break;
			}
			if ((!reversed && (firstElement - nextElement != -1)) || (reversed && (firstElement - nextElement != 1))) {
				isContinuous = false;
				checkedElements.clear();
			}
		}
		if (isContinuous) {
			if (find(calculatedElements.begin(), calculatedElements.end(), checkedElements) == calculatedElements.end()) {
				++continuousCols;
				calculatedElements.push_back(checkedElements);
			}
		}
		checkedElements.clear();
	}
	return continuousCols;
}