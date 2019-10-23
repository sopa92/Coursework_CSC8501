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
int FindContinuousRows(Puzzle* puzzle, bool reversed);
int FindContinuousCols(Puzzle* puzzle, bool reversed);
bool startOverGame();
void ReadPuzzleFromFileAndMoveAround();
void MoveAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence);
//void MoveAroundB(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, vector<string> puzzleStates, int& cRows, int& cCols, int& rcRows, int& rcCols, vector<string> movesSequences, string& moveSequence);
void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle);
bool turnHasCompleted(Puzzle* puzzle);
void readPuzzleArrayFromFile(Puzzle* puz, vector<string>& fullRows, string fullRowString, int& fileRow, int& i);
void PrintContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal);
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
			//SettingValuesManually(&mainPuzzle);
			SettingValuesAuto(&mainPuzzle);
			cout << mainPuzzle << endl;
		}
		else if(response==2)
			CreatePuzzleConfigurationsRandomly();
		else
			ReadPuzzleFromFileAndMoveAround();
		
		//startMovingRandomly(&mainPuzzle);

		startOver = startOverGame();
	}
	return 0;
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
		cout << "Results are now written in the Solution-File.txt !" << endl;
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
	if (printAction == 'y') {
		cout << endl << *puzzle;
	}
	int prevPositionX = rows - 1;
	int prevPositionY = columns - 1;
	int continuousRowsPerTurn = 0, continuousColsPerTurn = 0, revContinuousRowsPerTurn = 0, revContinuousColsPerTurn = 0;
	int continuousRowsTotal = 0, continuousColsTotal = 0, revContinuousRowsTotal = 0, revContinuousColsTotal = 0;
	vector<string> movesSequences;
	int existingPath = 0;
	while (existingPath < 100) {
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
	
	if (printAction == 'y') {
		PrintContinuousElements(continuousRowsTotal, continuousColsTotal, revContinuousRowsTotal, revContinuousColsTotal);
	}
	resultsStrStream << "row: " << continuousRowsTotal << endl << "column: " << continuousColsTotal << endl;
	resultsStrStream << "reverse row: " << revContinuousRowsTotal << endl << "reverse column: " << revContinuousColsTotal << endl << endl;
	string results = resultsStrStream.str();
	return results;
}
//
//string moveAndCalculateContinuousB(Puzzle* puzzle, int count) {
//	std::ostringstream resultsStrStream;
//	resultsStrStream << count << "\n";
//	resultsStrStream << *puzzle;
//	//cout << endl << *puzzle;
//	int prevPositionX = rows - 1;
//	int prevPositionY = columns - 1;
//	int contRowsPerTurn = 0, contColsPerTurn = 0, revContRowsPerTurn = 0, revContColsPerTurn = 0;
//	int continuousRowsTotal = 0, continuousColsTotal = 0, revContinuousRowsTotal = 0, revContinuousColsTotal = 0;
//	vector<string> movesSequences;
//	int existingPath = 0;
//	while (existingPath<30) {
//		contRowsPerTurn = contColsPerTurn = revContRowsPerTurn = revContColsPerTurn = 0;
//		do {
//			cout << *puzzle;
//			cout << "------ \n";
//			vector<string> puzzleStates;
//			string moveSequence;
//			puzzleStates.push_back(puzzle->ToString());
//			MoveAround(puzzle, prevPositionX, prevPositionY, puzzleStates, contRowsPerTurn, revContRowsPerTurn, contColsPerTurn, revContColsPerTurn, movesSequences, moveSequence);
//		} while (!turnHasCompleted(puzzle));
//		continuousRowsTotal += contRowsPerTurn;
//		continuousColsTotal += contColsPerTurn;
//		revContinuousRowsTotal += revContRowsPerTurn;
//		revContinuousColsTotal += revContColsPerTurn;
//	}
//	//PrintContinuousElements(continuousRowsTotal, continuousColsTotal, revContinuousRowsTotal, revContinuousColsTotal);
//	resultsStrStream << "row: " << continuousRowsTotal << endl << "column: " << continuousColsTotal << endl;
//	resultsStrStream << "reverse row: " << revContinuousRowsTotal << endl << "reverse column: " << revContinuousColsTotal << endl << endl;
//	string results = resultsStrStream.str();
//	return results;
//}

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

//
//void MoveAroundB(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, vector<string> puzzleStates, int &cRows, int &cCols, int &rcRows, int &rcCols, vector<string> movesSequences, string &moveSequence) {
//	vector<char> availSwaps;
//	bool moved = false;
//	int zero_x = puzzle->Get_zero_x();
//	int zero_y = puzzle->Get_zero_y();
//	puzzle->GetAvailableSwaps(availSwaps, rows, columns, zero_x, zero_y);
//	int attempts = 1;
//	while (!moved) {
//		for (char direction : availSwaps) {
//			Puzzle* newStatePuzzle = new Puzzle(puzzle);
//			moved = (*newStatePuzzle).MoveRandomlyAround(direction, zero_x, zero_y, newStatePuzzle, prevPositionX, prevPositionY, attempts);
//			if (!moved) {
//				vector<char>::iterator itr = find(availSwaps.begin(), availSwaps.end(), direction);
//				if (itr != availSwaps.end())
//				{
//					int index = itr - availSwaps.begin();
//					availSwaps.erase(availSwaps.begin() + (index));
//				}
//			}
//			else {
//				cout << direction << "\n";
//				cout << *newStatePuzzle;
//				cout << "________ \n";
//				moveSequence += direction;
//				prevPositionX = zero_x;
//				prevPositionY = zero_y;
//				string stateString = newStatePuzzle->ToString();
//				vector<string>::iterator iter = find(puzzleStates.begin(), puzzleStates.end(), stateString);
//				if (iter == puzzleStates.end())
//				{
//					puzzleStates.push_back(stateString);
//					FindContinuousElements(cRows, rcRows, cCols, rcCols, newStatePuzzle);
//					if (turnHasCompleted(newStatePuzzle)) {
//						if (find(movesSequences.begin(), movesSequences.end(), moveSequence) == movesSequences.end())
//						{
//							movesSequences.push_back(moveSequence);	
//							moveSequence.clear();
//						}
//						else {
//							cRows = cCols = rcRows = rcCols = 0;
//							moveSequence.clear();
//							break;
//						}
//					}
//					MoveAround(newStatePuzzle, prevPositionX, prevPositionY, puzzleStates, cRows, cCols, rcRows, rcCols, movesSequences, moveSequence);
//				}
//				else {
//					cout << "repetition \n";
//					cout << "_________________ \n";
//				}
//			}
//			delete newStatePuzzle;
//			newStatePuzzle = NULL;
//		}
//	}
//	availSwaps.clear();
//}

bool startOverGame() {
	cout << "Do you want to start over? (Y/N)" << endl;
	char answer = GetInputOption('Y', 'N');
	if (answer == 'y') {
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

void swap(int* a, int* b) {
	int temp = *a; 
	*a = *b;
	*b = temp;
}
void selectionSort(int arr[], int n)
{
	int i, j, min_idx;

	// One by one move boundary of unsorted subarray  
	for (i = 0; i < n - 1; i++)
	{
		// Find the minimum element in unsorted array  
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element  
		swap(&arr[min_idx], &arr[i]);
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
	if (printAction == 'y') {
		cout << inputAmount << endl;
	}
	for (int i = 0; i < inputAmount; ++i) {
		Puzzle randPuzzle;
		SettingValuesAuto(&randPuzzle);
		my15file << randPuzzle << endl;
		if (printAction == 'y') {
			cout << randPuzzle << endl;
		}
	}
	cout << "Generated puzzles are succesfully written in the 15-File.txt !" << endl;
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
	return answer;
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
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - + - +" << endl;
	cout << "| W | e | l | c | o | m | e |  | a | b | o | a | r | d | ! |" << endl;
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - + - +" << endl;
	cout << endl;
	cout << "What do you wish to do? Choose a number from below." << endl;
	cout << " 1 - I want to create my own 15-puzzle." << endl;
	cout << " 2 - I want you to create some 15-puzzles." << endl;
	cout << " 3 - I want to read 15-puzzles from a file and deduce continuous elements in all their turns." << endl;
}

void FindContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle) {
	contRows += FindContinuousRows(puzzle, false);
	revContRows += FindContinuousRows(puzzle, true);
	contCols += FindContinuousCols(puzzle, false);
	revContCols += FindContinuousCols(puzzle, true);
}

void PrintContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal) {
	cout << "row: " << contRowsTotal << endl;
	cout << "column: " << contColsTotal << endl;
	cout << "reverse row: " << revContRowsTotal << endl;
	cout << "reverse column: " << revContColsTotal << endl;
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
			if (!reversed) {
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
			if (!reversed) {
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
		}
	}
	return continuousCols;
}
