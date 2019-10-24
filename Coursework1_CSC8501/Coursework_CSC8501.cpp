//Course:		CSC8501
//Project:		Coursework part1 + part 2
//Author:		Sofia Papadopoulou - 190338075
//Acad. Year:	2019-2020
//Git Repo:		https://github.com/sopa92/Coursework_CSC8501.git
//References:	http://www.cplusplus.com, https://www.stackoverflow.com, https://www.geeksforgeeks.org, https://www.jstor.org (websites)
//				The C++ Standard Library - Nicolai M. Josuttis (book)


#include "Puzzle.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <regex>
#include <functional>

using namespace std;
template <typename T>
void printMenu(T dimension);
int getInputNumber(int min, int max);
int getInputNumber(Puzzle* puzzleArr);
char getInputOption(char optionA, char optionB);
void chooseManualOrAutoWay(Puzzle* puzzle);
void settingValuesManually(Puzzle* puzzle, int puzzleBlocks);
void createPuzzleConfigurationsRandomly();
void settingValuesAuto(Puzzle* puzzle, int row, int col);
void readPuzzleFromFileAndMoveAround();
string askForFilename();
void readPuzzleArrayFromFile(Puzzle* puz, vector<string>& fullRows, string fullRowString, int& fileRow, int& i);
string moveAndCalculateContinuous(Puzzle* puzzle, int count, bool printAction);
void moveRandomlyAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence);
void findContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle, vector<string>& calculatedElements);
int findContinuousRows(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements);
int findContinuousCols(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements);
bool turnHasCompleted(Puzzle* puzzle);
void printContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal);
int askUserForNPartialContinuousElements();
int findNPartialContinuousElements(Puzzle* mainPuzzle, int N, bool returnSumResult, bool printOnScreen);
void generateNPuzzleAndFindContinuous();
bool startOverGame();
int rows, columns;

int main()
{	
	bool startOver = true;
	int dimension = 4;
	int default_blocks = 15;
	printMenu(default_blocks);
	
	while (startOver) {
		int response = getInputNumber(1, 4);

		Puzzle mainPuzzle(dimension, dimension);
		rows = columns = dimension;

		if (response == 1) {
			chooseManualOrAutoWay(&mainPuzzle);
		}
		else if (response == 2) {
			createPuzzleConfigurationsRandomly();
		}
		else if (response == 3) {
			readPuzzleFromFileAndMoveAround();
		}
		else {
			cout << "What are the dimensions you want? Type a number between 2 and 100" << endl;
			int N = getInputNumber(2, 100);
			dimension = N;
			startOver = true;
			system("CLS");
			printMenu('N');
			continue;
		}

		startOver = startOverGame();
		if(startOver) 
			printMenu(default_blocks);
	}
	return 0;
}

template <typename T>
void printMenu(T blocks) {
	cout << endl;
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - +" << endl;
	cout << "| W | e | l | c | o | m | e |  | a | b | o | a | r | d |" << endl;
	cout << "+ - + - + - + - + - + - + - +  + - + - + - + - + - + - +" << endl;
	cout << endl;
	cout << "What do you wish to do? Choose a number from below." << endl;
	cout << " 1 - I want to create my own "<< blocks <<"-puzzle." << endl;
	cout << " 2 - I want you to generate some " << blocks << "-puzzles and write them in a file." << endl;
	cout << " 3 - I want you to read " << blocks << "-puzzles from a file and deduce the continuous elements." << endl;
	cout << " 4 - I want to use different dimensions' puzzle." << endl;
}

int getInputNumber(int min, int max) {
	int givenInput = 0;
	cout << "Type a number and press Enter:" << endl;
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

int getInputNumber(Puzzle* puzzleArr) {
	function<bool(int, Puzzle*)> isExistingNumber = [](int givenInput, Puzzle* puzzleArr) {
		for (int i = 0; i < rows; ++i) { for (int j = 0; j < columns; ++j) { if (puzzleArr->Get_element(i, j) == givenInput) return true; } } return false;
	};
	int puzzleBlocks = (rows * rows) - 1;
	int givenInput = 0;
	cout << "Type your chosen number and press Enter:" << endl;
	cin >> givenInput;
	while (true) {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please type a number between 1 and " << puzzleBlocks + 5 << "." << endl;
			cin >> givenInput;
		}
		else if (givenInput < 1 || givenInput >(puzzleBlocks + 5)) {
			cout << "You should type a number between 1 and " << puzzleBlocks + 5 << "." << endl;
			cin >> givenInput;
		}
		else if (isExistingNumber(givenInput, puzzleArr)) {
			cout << "You must not choose an existing number. \nPlease type another number between 1 and " << puzzleBlocks + 5 << "." << endl;
			cin >> givenInput;
		}
		else {
			break;
		}
	}

	return givenInput;
}

char getInputOption(char optionA, char optionB) {
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

void chooseManualOrAutoWay(Puzzle* puzzle) {
	int puzzleBlocks = (rows * rows) - 1;
	if (puzzleBlocks < 25) {
		settingValuesManually(puzzle, puzzleBlocks);
	}
	else{
		cout << "Are you sure you want to type in all those numbers? (Y/N)" << endl;
		char certainAnswer = getInputOption('Y', 'N');
		if (certainAnswer == 'N') {
			cout << "Do you want me to generate a puzzle for you? (Y/N)" << endl;
			char autoGenAnswer = getInputOption('Y', 'N');
			if (autoGenAnswer == 'Y') {
				settingValuesAuto(puzzle, rows, columns);
				cout << *puzzle << endl;
				int N = 0;
				do {
					N = askUserForNPartialContinuousElements();
					if (N > 0)
						findNPartialContinuousElements(puzzle, N, false, true);
				} while (N != 0);
			}
			return;
		}
		else {
			settingValuesManually(puzzle, puzzleBlocks);
		}
	}
}

void settingValuesManually(Puzzle* puzzle, int puzzleBlocks) {
	
	cout << "Please give me "<< puzzleBlocks <<" numbers between 1 and "<< puzzleBlocks+5 << "." << endl;
	string printedArray;
	for (int i = 0; i < puzzleBlocks; ++i) {
		cout << "| " << i+1 << " " ;
		printedArray += "| __ ";
	}
	cout << "|" << endl;
	printedArray += "|";
	cout << "----------------------------------------------------------------------------" << endl;
	string unfilledGap = "__";
	cout << printedArray << endl;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (i == rows - 1 && j == columns - 1) {
				puzzle->Set_element(i, j, 0);
				puzzle->Set_zero_position(i, j);
				break;
			}
			puzzle->Set_element(i, j, getInputNumber(puzzle));
			string newElement = std::to_string(puzzle->Get_element(i, j));
			printedArray.replace(printedArray.find(unfilledGap), unfilledGap.length(), newElement);
			cout << printedArray << endl;
		}
	}
	cout << "Thank you! The " << puzzleBlocks << "-puzzle is now ready!" << endl;
	cout << *puzzle << endl;

	int N = askUserForNPartialContinuousElements();
	if (N > 0)
		findNPartialContinuousElements(puzzle, N, false, true);
}

void createPuzzleConfigurationsRandomly() {
	int puzzleBlocks = (rows * rows) - 1;
	cout << "How many "<< puzzleBlocks << "-puzzle configurations do you want me to create?" << endl;
	int inputAmount = getInputNumber(0, 0);
	string filename = askForFilename();
	cout << "Do you want me also to print the configurations, apart from exporting them in a file? (Y/N)" << endl;
	char printAction = getInputOption('Y', 'N');
	ofstream myNfile;	
	myNfile.open(filename.c_str());
	myNfile << inputAmount << endl;
	if (printAction == 'Y') {
		cout << inputAmount << endl;
	}
	for (int i = 0; i < inputAmount; ++i) {
		Puzzle randPuzzle(rows, columns);
		settingValuesAuto(&randPuzzle, rows, columns);
		myNfile << randPuzzle << endl;
		if (printAction == 'Y') {
			cout << randPuzzle << endl;
		}
		else if (inputAmount > 5000 and 0 == i % 100)
			cout << ". . . ";
	}
	cout << endl << "Generated puzzles are succesfully written in the "<< filename <<" !" << endl;
	myNfile.close();
}

void settingValuesAuto(Puzzle* puzzle, int row, int col) {
	int numbersNeeded = row * row;
	int* randArray = new int[numbersNeeded];
	for (int i = 0; i < numbersNeeded; ++i) {
		randArray[i] = i + 1;
	}
	random_shuffle(randArray, randArray + numbersNeeded);
	int k = 0;
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			if (i == row - 1 && j == col - 1) {
				puzzle->Set_element(i, j, 0);
				puzzle->Set_zero_position(i, j);
				break;
			}
			puzzle->Set_element(i, j, randArray[k]);
			k++;
		}
	}
	delete[] randArray;
	randArray = NULL;
}


void readPuzzleFromFileAndMoveAround() {
	
		string filename = askForFilename();
		ifstream my15file;
		string fullRowString;
		istringstream iss(fullRowString);
		try {
			my15file.open(filename.c_str());
			if (my15file.fail())
				throw invalid_argument("There is no such file, as " + filename + ".");
		}
		catch (const invalid_argument& ex) {
			cout << ex.what() << endl << "In order to use a file, you have to create it first." << endl;
			my15file.close();
			return;
		}
		cout << "Do you want me also to print the configurations, apart from exporting them in a file? (Y/N)" << endl;
		char printAction = getInputOption('Y', 'N');
		vector<string> fullRows;
		Puzzle puz(rows,columns);
		int i = 0, fileRow = 0, countPuzzles = 0;
		int minRowSize = (rows * 2) - 2;
		string resultsForFile;
		while (getline(my15file, fullRowString)) {
			if (i >= rows) {
				++countPuzzles;;				
				resultsForFile += moveAndCalculateContinuous(&puz, countPuzzles, printAction == 'Y');								
				Puzzle puz(rows, columns);
				i = 0;
			}
			if (fullRowString.size() > minRowSize && i < rows) {
				try {
					readPuzzleArrayFromFile(&puz, fullRows, fullRowString, fileRow, i);
				}
				catch (const invalid_argument& e) {
					cout << e.what() << endl << "Terminating..." << endl;
					my15file.close();
					return;
				}
			}
		}
		my15file.close();

		ofstream solutionFile;
		solutionFile.open("Solution-File.txt");
		solutionFile << resultsForFile;
		cout << endl << "Results are now written in the Solution-File.txt !" << endl;
		solutionFile.close();
}

string askForFilename() {
	cout << "What is the name of the file you want me to use?" << endl;
	string answer;
	cin >> answer;
	regex rgx_filename("[a-zA-Z0-9 _-]+\\.[A-Za-z]{3}");
	while (!regex_match(answer, rgx_filename)) {
		cout << "Invalid filename. Please type in a valid filename" << endl;
		cin >> answer;
	}
	return answer;
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
			if (intValue == 0) {
				string errorString = "Invalid value found in line " + to_string(i + 2);
				throw invalid_argument(errorString);
			}
			if (!fullRows.empty())
				puz->Set_element(i, j, intValue);
		}
	}
	++i;
	++fileRow;
}

string moveAndCalculateContinuous(Puzzle* puzzle, int count, bool printAction) {

	std::ostringstream resultsStrStream;
	resultsStrStream << count << "\n";
	resultsStrStream << *puzzle;
	int prevPositionX = rows - 1;
	int prevPositionY = columns - 1;
	int continuousRowsPerTurn = 0, continuousColsPerTurn = 0, revContinuousRowsPerTurn = 0, revContinuousColsPerTurn = 0;
	int continuousRowsTotal = 0, continuousColsTotal = 0, revContinuousRowsTotal = 0, revContinuousColsTotal = 0;
	vector<string> movesSequences, calculatedElements;
	int nContElementsSum = 0;
	int Partial2onConfig, Partial3onConfig, Partial4onConfig, Partial2onTurn = 0, Partial3onTurn = 0, Partial4onTurn = 0;
	Partial2onConfig = findNPartialContinuousElements(puzzle, 2, true, false);
	Partial3onConfig = findNPartialContinuousElements(puzzle, 3, true, false);
	Partial4onConfig = findNPartialContinuousElements(puzzle, 4, true, false);

	if (printAction) {
		cout << endl << *puzzle;
	}
	string moveSequence;
	do {
		moveRandomlyAround(puzzle, prevPositionX, prevPositionY, moveSequence);
		findContinuousElements(continuousRowsPerTurn, revContinuousRowsPerTurn, continuousColsPerTurn, revContinuousColsPerTurn, puzzle, calculatedElements);
		Partial2onTurn += findNPartialContinuousElements(puzzle, 2, true, false);
		Partial3onTurn += findNPartialContinuousElements(puzzle, 3, true, false);
		if (turnHasCompleted(puzzle)) {
			if (find(movesSequences.begin(), movesSequences.end(), moveSequence) == movesSequences.end())
			{
				movesSequences.push_back(moveSequence);
			}
			else {
				continuousRowsPerTurn = continuousColsPerTurn = revContinuousRowsPerTurn = revContinuousColsPerTurn = 0;
				break;
			}
		}
	} while (!turnHasCompleted(puzzle));
	continuousRowsTotal += continuousRowsPerTurn;
	continuousColsTotal += continuousColsPerTurn;
	revContinuousRowsTotal += revContinuousRowsPerTurn;
	revContinuousColsTotal += revContinuousColsPerTurn;
	Partial4onTurn = continuousRowsTotal + continuousColsTotal + revContinuousRowsTotal + revContinuousColsTotal;

	if (printAction) {
		printContinuousElements(continuousRowsTotal, continuousColsTotal, revContinuousRowsTotal, revContinuousColsTotal);
		cout << "(total for row & column, including reverse, in this configuration) \n2 = " << Partial2onConfig << "\n3 = " << Partial3onConfig << "\n4 = " << Partial4onConfig << endl;
		cout << "(total for row and column, including reverse, for all valid turns) \n2 = " << Partial2onTurn << "\n3 = " << Partial3onTurn << "\n4 = " << Partial4onTurn << endl << endl;
	}
	resultsStrStream << "row: " << continuousRowsTotal << endl << "column: " << continuousColsTotal << endl;
	resultsStrStream << "reverse row: " << revContinuousRowsTotal << endl << "reverse column: " << revContinuousColsTotal << endl;
	resultsStrStream << "(total for row & column, including reverse, in this configuration) \n2 = " << Partial2onConfig << "\n3 = " << Partial3onConfig << "\n4 = " << Partial4onConfig << endl;
	resultsStrStream << "(total for row and column, including reverse, for all valid turns) \n2 = " << Partial2onTurn << "\n3 = " << Partial3onTurn << "\n4 = " << Partial4onTurn << endl << endl;
	string results = resultsStrStream.str();
	return results;
}

void moveRandomlyAround(Puzzle* puzzle, int& prevPositionX, int& prevPositionY, string& moveSequence) {
	vector<char> availSwaps;
	bool moved = false;
	int zero_x = puzzle->Get_zero_x();
	int zero_y = puzzle->Get_zero_y();
	puzzle->GetAvailableSwaps(availSwaps, rows, columns, zero_x, zero_y);
	int attempts = 1;
	while (!moved) {
		int randValue = rand() % availSwaps.size();
		char direction = availSwaps[randValue];
		moved = puzzle->MoveBlock(direction, zero_x, zero_y, puzzle, prevPositionX, prevPositionY, attempts);
		if (!moved) {
			availSwaps.erase(availSwaps.begin() + (randValue));
			if (rows > 15) cout << ". ";
		}
		else {
			moveSequence += direction;
			prevPositionX = zero_x;
			prevPositionY = zero_y;
		}
	}
	availSwaps.clear();
}

void findContinuousElements(int& contRows, int& revContRows, int& contCols, int& revContCols, Puzzle* puzzle, vector<string>& calculatedElements) {
	contRows += findContinuousRows(puzzle, false, calculatedElements);
	revContRows += findContinuousRows(puzzle, true, calculatedElements);
	contCols += findContinuousCols(puzzle, false, calculatedElements);
	revContCols += findContinuousCols(puzzle, true, calculatedElements);
}

int findContinuousRows(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements) {
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

int findContinuousCols(Puzzle* puzzle, bool reversed, vector<string>& calculatedElements) {
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

bool turnHasCompleted(Puzzle* puzzle) {
	return (puzzle->Get_zero_x() == rows - 1 && puzzle->Get_zero_y() == columns - 1);
}

void printContinuousElements(int& contRowsTotal, int& contColsTotal, int& revContRowsTotal, int& revContColsTotal) {
	cout << "row: " << contRowsTotal << endl;
	cout << "column: " << contColsTotal << endl;
	cout << "reverse row: " << revContRowsTotal << endl;
	cout << "reverse column: " << revContColsTotal << endl;
}

int askUserForNPartialContinuousElements() {
	cout << "Do you want me to calculate how many N-partial continuous rows, columns, \nreverse rows and reverse columns exist in this configuration? (Y/N)" << endl;
	char answer = getInputOption('Y', 'N');
	if (answer == 'N') {
		return 0;
	}
	cout << "What will be the N?" << endl;
	int N = getInputNumber(2, 4);
	while (N > rows) { 
		cout << "The N you chose is bigger than the dimensions of the puzzle. \nThere are no " << N << "-partial continuous elements in a " << rows << " by " << rows << " puzzle." << endl;
		N = getInputNumber(2, 4);
	}
	return N;
}

int findNPartialContinuousElements(Puzzle* mainPuzzle, int N, bool returnSumResult, bool printOnScreen) {

	int nPartialRows = 0, nPartialCols = 0, nPartialRevRows = 0, nPartialRevCols = 0;
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
			findContinuousElements(nPartialRows, nPartialRevRows, nPartialCols, nPartialRevCols, miniPuzzle, calculatedElements);
			delete miniPuzzle;
			miniPuzzle = NULL;
		}
	}
	
	if (returnSumResult) {
		return (nPartialRows + nPartialCols + nPartialRevRows + nPartialRevCols);
	}
	if(printOnScreen)
		printContinuousElements(nPartialRows, nPartialCols, nPartialRevRows, nPartialRevCols);
	
	return 0;
}

void generateNPuzzleAndFindContinuous() {
	cout << "What are the dimensions you want? Type a number between 2 and 200";
	int N = getInputNumber(2, 200);
	Puzzle* nPuzzle = new Puzzle(N, N);
	rows = columns = N;
	settingValuesAuto(nPuzzle, N, N);
	cout << *nPuzzle << endl;
	int Partial2onConfig = findNPartialContinuousElements(nPuzzle, 2, true, false);
	int Partial3onConfig = findNPartialContinuousElements(nPuzzle, 3, true, false);
	int Partial4onConfig = findNPartialContinuousElements(nPuzzle, 4, true, false);
	cout << "(total for row & column, including reverse, in this configuration) \n2 = " << Partial2onConfig << "\n3 = " << Partial3onConfig << "\n4 = " << Partial4onConfig << endl;
}

bool startOverGame() {
	cout << "Do you want to start over? (Y/N)" << endl;
	char answer = getInputOption('Y', 'N');
	if (answer == 'Y') {
		system("CLS");
		return true;
	}
	return false;
}
