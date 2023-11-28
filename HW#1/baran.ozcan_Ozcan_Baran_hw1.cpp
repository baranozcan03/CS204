// Baran Ozcan, 30960 

//libraries that have been included
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Namespace for convinece.
using namespace std;

//Function prototypes. The explanations on funcitons given when defining their use, not here!
string FileChecker(int&, int&);
vector<vector<char>> MatrixMaker(string, int);
vector<string> ListMaker(string, int, int&);
bool Hunt(string, vector<vector<char>>&);
bool Search(string, vector<vector<char>>&, int, int);
bool fitter(int, int, int, bool, string, string, int);
bool FinalChecker(int, int, int, bool, int, string, string, vector<vector<char>>&, string);

//preconditon= a series of inputs that will be reuqested
//funciton= given a file that obeys ceratin constraints,
//finds the number of occurances and words that occured in a matrix in spiral direcitons
//words and matix are provided by the user
//postondition=if unvalid file is give, error message. If the file is valid, number of words found and the words themselves  
int main()
{
	//Check wheter the input file is valid, then give out the name of the file, matrixsize and number of words 
	string FileName;
	int matrixSize, numberOfWords;
	FileName = FileChecker(matrixSize, numberOfWords);

	if (FileName != "ERROR") {
		//Define vectors for Matrix and Words.
		vector<vector<char>> Matrix(matrixSize, vector<char>(matrixSize));
		vector<string> WordsList(numberOfWords);

		//Fill in the matrix
		Matrix = MatrixMaker(FileName, matrixSize);
		//Fill in the words vector
		WordsList = ListMaker(FileName, matrixSize, numberOfWords);
		//start a counter with the updated word count 
		vector<int> CounterList(numberOfWords, 0);
		
		
		//start the word hunt for every word
		for (int i = 0; i < numberOfWords; i++) {
			//if the word found in the matrix update its counter.
			if (Hunt(WordsList[i], Matrix)) {
				CounterList[i] = 1;
			}
		}

		//Cout findings
		//find the amount of words found
		int grand_total = 0;
		for (int i = 0; i < CounterList.size(); i++) {
			grand_total += CounterList[i];
		}
		cout << grand_total << " Words are Found: ";
		//print the words
		for (int i = 0; i < CounterList.size(); i++) {
			if (CounterList[i] == 1) {
				cout << " " << WordsList[i];
			}
		}

	}
	return 0;
}

//precondition = matrixSize and wordNumber. (given as referance prameter to save them for later use)
//function1 = Checks whether the file has nxn matrix and words consisting of all uppercase latters.
//function2 = Checks whether the given filename exist in the directory. 
//function3 = Checks whether the number of search words given matches the number of search words in the file.
//postcondition = Return filename,(matrix size and word number as referance). "ERROR" string if the file is not correct.
string FileChecker(int& matrixSize, int& numberOfWords) {

	//define the ifstream
	ifstream input;

	//Try to open the folder and see if its there. If not cout  an error message and contiune asking until a correct filename is entered.
	string FileName;
	while (true) {
		cout << "Enter the name of the file" << endl;
		cin >> FileName;
		input.open(FileName); // used to use "input.open(FileName.c_str());", not sure if revert back
		// the file is assumed to be in the same directory. 
		if (input.fail()) {
			cout << "Could not open the file " << FileName << endl;
		}
		else {
			break;
		}
	}


	//check whether the size of the matrix is an intiger, if not promtp an error message, if yes store it.
	if (!(input >> matrixSize)) {
		cout << "Error: Input file is not in correct format!" << endl;
		return "ERROR";
	}
	//then get rid of endl chr.
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	//take a row by getline and see if it includes capital letters and has length of 'matrixSize'.
	//do this matrixSize times fo all lines.
	string theRow;
	for (int i = 0; i < matrixSize; i++) {

		getline(input, theRow);
		//check the length
		if (theRow.length() != matrixSize) {
			cout << "Error: Input file is not in correct format!" << endl;
			return "ERROR";
		}
		//check the capital letters
		for (int l = 0; l < theRow.length(); l++) {
			if (!isupper(theRow[l])) {
				cout << "Error: Input file is not in correct format!" << endl;
				return "ERROR";
			}
		}
	}


	//check whether the number of words is an intiger, if not promtp an error message, if it is store it
	if (!(input >> numberOfWords)) {
		cout << "Error: Input file is not in correct format!" << endl;
		return "ERROR";
	}
	//then get rid of endl chr.
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	//Take a row by getline and see if it includes capital letters. Do this numberOfWords times.
	for (int i = 0; i < numberOfWords; i++) {
		getline(input, theRow);
		//check the capital letters
		for (int l = 0; l < theRow.length(); l++) {
			if (!isupper(theRow[l])) {
				cout << "Error: Input file is not in correct format!" << endl;
				return "ERROR";
			}
		}
	}
	return FileName;
}

//precondition = File name and matrix size.
//function = Will read the file that has passed input check, take the matrix part and convert it into a matrix.
//postcondition = Will return a matrix vector that has read from the file. 
vector<vector<char>> MatrixMaker(string FileName, int matrixSize) {

	//initialize matrix and input stream.
	vector<vector<char>> Matrix(matrixSize, vector<char>(matrixSize));
	ifstream input;

	//open the file and discard the first line.
	input.open(FileName.c_str());
	string theRow;
	getline(input, theRow);

	//start reading the lines, matrix size times, matrix is alreadly valid so no checks here.
	for (int i = 0; i < matrixSize; i++) {
		getline(input, theRow);
		for (int l = 0; l < matrixSize; l++) {
			Matrix[i][l] = theRow[l];
		}
	}

	return Matrix;
}

//precondition: Filename the list belongs, number of matrix rows, number of words to be read
//function: skips the matrix part and reads the word list part in to a vector
//postconditon: A vector that has all the words, also we update word number for uniqueness
vector<string> ListMaker(string FileName, int matrixSize, int&wordNumbers) {
	//define the return vecor and the stream of input.
	vector<string> WordsList;
	ifstream input;

	//open the file and discard the first matrixsize+2 lines (matrixSize,matrix,wordsNum).
	input.open(FileName.c_str());
	string theRow;
	for (int i = 0; i < matrixSize + 2; i++) {
		getline(input, theRow);
	}

	//start reading the words, wordNumbers times, it  so only 1 check
	for (int k = 0; k < wordNumbers; k++) {
		getline(input, theRow);
		bool doesExist = false; //if the word is already in the list dont add it
		for (int i = 0; i < WordsList.size(); i++) {
			if (theRow == WordsList[i]) {
				doesExist = true;
			}
		}
		if (!doesExist) {
			WordsList.push_back(theRow);
		}
	}

	//change the number of words for unique vlaues
	wordNumbers = WordsList.size();

	return WordsList;
}

//precondition= the word of interest, and the matrix board that we are currently on
//function= searches the word in any point on the board on both clockwise and counter clockwise spirals.
//notes: 4 directions and 2 ways, a total of 8 paths to follow.
//postcondition= false if the word is not found, true if found.
bool Hunt(string Word, vector<vector<char>>& Matrix) {
	//initial letter of the word for checking starting point.
	char initial = Word[0];

	//look for the places in the matrix where the words initial letters are, if found initiate a search.
	for (int i = 0; i < Matrix.size(); i++) {
		for (int l = 0; l < Matrix.size(); l++) {
			if (Matrix[i][l] == initial) {
				//start a 8 directional search in matrix, if you find the word return true.
				if (Search(Word, Matrix, l, i)) {
					return true;
				}
			}
		}
	}
	//return false if not found
	return false;
}


//precondition: our matrix as the board, the word we will searh for, and the starting coordinates on the board.
//funciton: will do a 8 path search for the word
//postcondition: will return true if the word is found, false if not found.
bool Search(string Word, vector<vector<char>>& Matrix, int x, int y) {

	//if length 1 thats already ture
	if (Word.length() == 1) {
		return true;
	}

	//we will calculate the number of movements needed, we already have 1 letter 
	int moves = Word.length() - 1;

	//we know the step size of the search goes like:1,1,2,2,3,3,4,4... 
	//Find how many straigth strokes we will go then the redundant last move.
	//Strokenumbers = 2*n*(n+1) / 2 = n^2+n = move that n pair strokes spend. 
	int strokeP = 0;
	while (((strokeP * strokeP + strokeP) <= moves)) {
		strokeP++;
	}
	strokeP--;
	int redundant = moves - (strokeP * strokeP + strokeP);
	bool HalfStrokeP = false;
	if ((strokeP + 1) <= redundant) {
		HalfStrokeP = true;
		redundant -= strokeP + 1;
	}
	//strokeP=number of pair strokes, HalfStrokeP=one extra stroke, 
	//redundant=remaining of the stroke pair but not complete.

	//define the directions that will be taken in a vector
	vector<vector<string>> DirectionList(8, vector<string>(2));
	DirectionList[0] = { "N","C" };
	DirectionList[1] = { "N","CC" };
	DirectionList[2] = { "S","C" };
	DirectionList[3] = { "S","CC" };
	DirectionList[4] = { "W","C" };
	DirectionList[5] = { "W","CC" };
	DirectionList[6] = { "E","C" };
	DirectionList[7] = { "E","CC" };

	//start the search for each individual directions in a loop
	bool DoesExist = false;
	for (int i = 0; i < 8; i++) {
		if (fitter(x, y, strokeP, HalfStrokeP, DirectionList[i][0], DirectionList[i][1], Matrix.size())) {
			DoesExist = FinalChecker(x, y, strokeP, HalfStrokeP, redundant, DirectionList[i][0], DirectionList[i][1], Matrix, Word);
		}
		if (DoesExist) { return true; }
	}

	return false;
}


//precondition:the position of the search, the moves that will be done, the directions
//funciton: decides whether the search fits in the board, therefore possible or not
//postcondition: returns false if the search is not possible, true if it is possible
bool fitter(int x, int y, int StrokeP, bool HalfStrokeP, string cardinal, string rotation, int MatrixSize) {
	//number of comlete layers and quarter layers around the start position,
	//in which direction to andvence how much
	int completeLayer, quarterLayer, North = 0, South = 0, East = 0, West = 0;

	//each stroke pair will require 2 new extensions on the sphere;
	//each halfStroke will require 1 while redundant requires 0;
	int extensions = 2 * StrokeP;
	if (HalfStrokeP) { extensions + 1; }

	//now check the borders and see if they fit,while considering direcitons
	//first calculate complete layers, than add half ones according to direction
	completeLayer = extensions / 4;
	North += completeLayer;
	South += completeLayer;
	East += completeLayer;
	West += completeLayer;

	if (cardinal == "N" && rotation == "C") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { North++; }
		else if (quarterLayer == 2) { East++; North++; }
		else if (quarterLayer == 3) { South++; East++; North++; }
	}
	else if (cardinal == "N" && rotation == "CC") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { North++; }
		else if (quarterLayer == 2) { West++; North++; }
		else if (quarterLayer == 3) { South++; West++; North++; }
	}
	else if (cardinal == "W" && rotation == "C") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { West++; }
		else if (quarterLayer == 2) { North++; West++; }
		else if (quarterLayer == 3) { East++; North++; West++; }
	}
	else if (cardinal == "W" && rotation == "CC") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { West++; }
		else if (quarterLayer == 2) { South++; West++; }
		else if (quarterLayer == 3) { East++; South++; West++; }
	}
	else if (cardinal == "S" && rotation == "C") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { South++; }
		else if (quarterLayer == 2) { West++; South++; }
		else if (quarterLayer == 3) { North++; West++; South++; }
	}
	else if (cardinal == "S" && rotation == "CC") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { South++; }
		else if (quarterLayer == 2) { East++; South++; }
		else if (quarterLayer == 3) { North++; East++; South++; }
	}
	else if (cardinal == "E" && rotation == "C") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { East++; }
		if (quarterLayer == 2) { South++; East++; }
		if (quarterLayer == 3) { West++; South++; East++; }
	}
	else if (cardinal == "E" && rotation == "CC") {
		quarterLayer = extensions % 4;
		if (quarterLayer == 1) { East++; }
		else if (quarterLayer == 2) { North++; East++; }
		else if (quarterLayer == 3) { West++; North++; East++; }
	}

	//now check whether it fits the board or not 
	//if it fits return true. 
	if ((x - West) >= 0 && (x + East) < MatrixSize) {
		if ((y - North) >= 0 && (y + South) < MatrixSize) {
			return true;
		}
	}

	return false;
}

//precondiiton: position of the start, how many strokes, way of the search
//matrix that we search on and the word that we saerch for 
//function: uses the logical move of a spiral to claculate next letter and compares it to the search word
//postcondition: true if the word exits in this praticullar path, flase is it doesnt
bool FinalChecker(int x, int y, int strokeP, bool HalfStrokeP, int redundant, string cardinal, string rotation, vector<vector<char>>& Matrix, string Word) {

	//at the start we havent moved so current counts are all 0.p=pair strokes,s=single strokes 
	int currentMove = 0, currentP = 0, currentS = 0, movesNeeded = Word.length() - 1;

	//direction of the way decided accoring to strokes (y=1,x=0)
	//clockwise movement from notrh =			(-y, +x, +y, -x) repeat
	//Counterclockwise movement from north =	(-y, -x, +y, +x) repeat
	//clockwise movement from south =			(+y, -x, -y, +x) repeat
	//Counterclockwise movement from south =	(+y, +x, -y, -x) repeat
	//clockwise movement from east =			(+x, +y, -x, -y) repeat
	//Counterclockwise movement from east =		(+x, -y, -x, +y) repeat
	//clockwise movement from west =			(-x, -y, +x, +y) repeat
	//Counterclockwise movement from west =		(-x, +y, +x, -y) repeat
	int Signs[8] = { 0,0,0,0,0,0,0,0 };
	if ((cardinal == "N") && (rotation == "C")) {
		int newSigns[8] = { -1, 1, 1, 0, 1, 1, -1, 0 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "N") && (rotation == "CC")) {
		int newSigns[8] = { -1,1,-1,0,1,1,1,0 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "S") && (rotation == "C")) {
		int newSigns[8] = { 1,1,-1,0,-1,1,1,0 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "S") && (rotation == "CC")) {
		int newSigns[8] = { 1,1,1,0,-1,1,-1,0 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "E") && (rotation == "C")) {
		int newSigns[8] = { 1,0,1,1,-1,0,-1,1 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "E") && (rotation == "CC")) {
		int newSigns[8] = { 1,0,-1,1,-1,0,1,1 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "W") && (rotation == "C")) {
		int newSigns[8] = { -1,0,-1,1,1,0,1,1 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}
	else if ((cardinal == "W") && (rotation == "CC")) {
		int newSigns[8] = { -1,0,1,1,1,0,-1,1 };
		copy(begin(newSigns), end(newSigns), begin(Signs));
	}

	//since we know in which way, given that we know where we start and the direciton
	//we can calculate the direction of our next strok, 
	//after each stroke is complete, in the array direcitons of the next stroke is given
	//since the path is spiral, we just  follow the same thing
	while (currentP != strokeP) {
		currentP++;
		for (int i = 0; i < currentP; i++) {
			if ((Signs[(2 * currentS % 8) + 1]) == 1) {
				y += Signs[(2 * currentS % 8)];
			}
			if ((Signs[((2 * currentS % 8) + 1)]) == 0) {
				x += Signs[(2 * currentS % 8)];
			}
			currentMove++;
			char control = Word[currentMove];
			if (Matrix[y][x] != control) {
				return false;
			}
		}
		currentS++;
		for (int l = 0; l < currentP; l++) {
			if ((Signs[(2 * currentS % 8) + 1]) == 1) {
				y += Signs[(2 * currentS % 8)];
			}
			if ((Signs[((2 * currentS % 8) + 1)]) == 0) {
				x += Signs[(2 * currentS % 8)];
			}
			currentMove++;
			char control = Word[currentMove];
			if (Matrix[y][x] != control) {
				return false;
			}
		}
		currentS++;
	}
	currentP++;
	if (HalfStrokeP) {
		for (int i = 0; i < currentP; i++) {
			if ((Signs[(2 * currentS % 8) + 1]) == 1) {
				y += Signs[(2 * currentS % 8)];
			}
			else if ((Signs[((2 * currentS % 8) + 1)]) == 0) {
				x += Signs[(2 * currentS % 8)];
			}
			currentMove++;
			char control = Word[currentMove];
			if (Matrix[y][x] != control) {
				return false;
			}
		}
		currentS++;
	}

	while (redundant != 0)
	{
		redundant--;
		if ((Signs[(2 * currentS % 8) + 1]) == 1) {
			y += Signs[(2 * currentS % 8)];
		}
		else if ((Signs[((2 * currentS % 8) + 1)]) == 0) {
			x += Signs[(2 * currentS % 8)];
		}
		currentMove++;
		char control = Word[currentMove];
		if (Matrix[y][x] != control) {
			return false;
		}
	}

	return true;
}