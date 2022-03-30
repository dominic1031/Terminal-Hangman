#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

#include "Difficulty.h"
#include "Game.h"
using namespace std;

// function declaration
void PrintTextFile(string textFileName);
void SaveGame(Game *gameObj, string nameOfSaveFile);
string RequestForValidChoice(string *validChoices, int size);
string GetRandomWord(string textFileName, int totalWords);
void ClearTextFile(string filename);
bool LoadGame(Game *gameObj, string nameOfSaveFile);

int main() {
	try {
		srand(time(NULL)); // generates a seed based on the seconds since Unix epoch
		while (true) {
			
			// THE MENU CODE //
			system("CLS"); // clears up any previous text
			PrintTextFile("MenuScreen.txt"); // prints menu screen
			int sizeOfMenuOptions = 3;
			string *menuOptions = new string[sizeOfMenuOptions];
			menuOptions[0] = "1"; menuOptions[1] = "2"; menuOptions[2] = "3";
			string currentChoice = RequestForValidChoice(menuOptions, sizeOfMenuOptions);  // stores the choice inputed by the user
			delete[] menuOptions;
			Game *hangman = new Game;
			system("CLS"); //clears the screen

			if (currentChoice == "1") { // New Game
				cout << "Select a Difficulty: \n";
				cout << "1: Easy \n";
				cout << "2: Medium \n";
				cout << "3: Hard \n";
				cout << "\n";
				int sizeOfDifficultyMenu = 3;
				string *difficultyMenuOptions = new string[sizeOfDifficultyMenu]; // holds the available options at the menu
				difficultyMenuOptions[0] = "1"; difficultyMenuOptions[1] = "2"; difficultyMenuOptions[2] = "3";
				currentChoice = RequestForValidChoice(difficultyMenuOptions, sizeOfDifficultyMenu);
				delete[] difficultyMenuOptions;

				// Sets the Difficulty to the corresponding option
				if (currentChoice == "1") { // Easy
					Difficulty easy(1, 5, 10);
					hangman->SetDifficulty(easy);
				}
				else if (currentChoice == "2") { // Medium
					Difficulty medium(5, 8, 6);
					hangman->SetDifficulty(medium);

				}
				else if (currentChoice == "3") { // Hard
					Difficulty hard(9, 50, 6);
					hangman->SetDifficulty(hard);
				}

				// Randomly selects words inside dictionary until it meets the required length
				// Selected word becomes the word the player has to guess
				system("CLS");
				cout << "Loading word..." << endl;
				do {
					const int totalWordsInDictionary = 84047;
					string randomWord = GetRandomWord("engmix.txt", totalWordsInDictionary);
					hangman->SetWordToGuess(randomWord);
				} while (hangman->GetWordToGuess().length() >= hangman->GetDifficulty().GetMaxAmountOfLetters() ||
					hangman->GetWordToGuess().length() <= hangman->GetDifficulty().GetMinAmountOfLetters());
				system("CLS");

				// Sets the word player sees as dashes, each dash represents an unknown letter
				// For example, if the word is apple, then the progressWord would be -----
				string temp = "";
				for (int i = 0; i < hangman->GetWordToGuess().length(); i++) {
					temp += "-";
				}
				hangman->SetProgressWord(temp);

				// Sets the number of attempts given the difficulty
				hangman->SetAttemptsLeft(hangman->GetDifficulty().GetNumberOfAttempts());

			}
			else if (currentChoice == "2") { // Continue Game
				cout << "Loading Save File..." << endl;
				if (LoadGame(hangman, "SaveFile.txt") == false) {
					cout << "Save data was not found." << endl;

					string quit;
					cout << "Enter any key to return to the main menu: ";
					cin >> quit;
					continue; // goes back to the main menu
				}
				else {
					cout << "Save data was found! Resuming game." << endl;
				}
			}
			else if (currentChoice == "3") { // Quit Game
				break;
			}
			else {
				throw runtime_error("Invalid option");
			}
			// GAMEPLAY CODE //
			while (hangman->GetAttemptsLeft() > 0) {
				bool hasWon = false;

				// Displays the art and instructions during the game
				hangman->Print("Drawings.txt");
				cout << endl;
				cout << "Word: " << hangman->GetProgressWord() << " " << endl << "Attempts left: " << hangman->GetAttemptsLeft() << endl;
				cout << "Enter one of the following: ";
				for (int i = 0; i < hangman->GetSizeOfAvailableSelections(); i++)
					cout << hangman->GetAvailableSelections()[i] << " ";
				cout << endl;
				cout << "Enter a letter to guess the letter!" << endl;
				cout << "Enter ? to guess the word!" << endl;
				cout << "Enter * to save the game!" << endl;


				// Takes an inputed guess from the user
				string guess = RequestForValidChoice(hangman->GetAvailableSelections(), hangman->GetSizeOfAvailableSelections());
				string wordGuess; // if the player chooses ?, then wordGuess will be the word that the player has guessed
				if (guess == "?") {
					cout << "Enter your guess: ";
					cin >> wordGuess;
				}

				system("CLS");

				// Saves the game whenever the player enters *. Then brings them back to the game.
				if (guess == "*") { 
					SaveGame(hangman, "SaveFile.txt");
					cout << "Game has been saved! You may safely close and resume the game later." << endl;
					continue;
				}

				// Loop will remove the guessed string from the available selections array, if the guess is a valid letter.
				// It does this by making a temp array with the correct elements 
				// And then setting the elements of available selection array to the elements of the temp array
				if (guess != "?") { 
					int tempArrSize = hangman->GetSizeOfAvailableSelections() - 1;
					string *tempArr = new string[tempArrSize];
					int tempIndex = 0;
					for (int i = 0; i < hangman->GetSizeOfAvailableSelections(); i++) {
						if (!(guess == hangman->GetAvailableSelections()[i])) {
							tempArr[tempIndex] = hangman->GetAvailableSelections()[i];
							tempIndex++;
						}
					}
					hangman->SetAvailableSelections(tempArr, tempArrSize);
					delete[] tempArr;
				}
				
				// Checks if guess/wordGuess is correct or not
				bool guessIsInWord = false;
				if (guess != "?") { 
					for (int i = 0; i < hangman->GetWordToGuess().length(); i++) {
						if (guess[0] == hangman->GetWordToGuess()[i]) {
							guessIsInWord = true;
							break;
						}
						guessIsInWord = false;
					}
				}
				else if (guess == "?") {
					if (wordGuess == hangman->GetWordToGuess()) {
						hasWon = true;
						guessIsInWord = true;
						hangman->SetProgressWord(hangman->GetWordToGuess());
					}
					else {
						guessIsInWord = false;
					}
				}

				// Based on whether the guess is correct or not, the progress word will be updated or number of attempts will be lowered
				if (guessIsInWord) {
					if (guess != "?")
						hangman->UpdateProgressWord(guess);
				}
				else {
					hangman->SetAttemptsLeft(hangman->GetAttemptsLeft()-1);
				}

				// Check for win condition (i.e. if there are no dashes (-) in progressWord i.e. if all letters are guessed)
				for (int i = 0; i < hangman->GetProgressWord().length(); i++) {
					if (hangman->GetProgressWord()[i] == '-') { // if one letter is a dash, hasWon = false and will break out of loop
						hasWon = false;
						break;
					}
					if (i == hangman->GetProgressWord().length() - 1) { //when all the letters are checked to have no dashes, hasWon = true
						hasWon = true;
					}
				}
				if (hasWon) {
					// PUT WINNING CODE HERE
					hangman->Print("Drawings.txt");
					cout << endl;
					cout << "You win!" << endl;
					cout << "The word was " << hangman->GetWordToGuess() << "." << endl;
				
					string quit;
					cout << "Enter any key to return to the main menu: ";
					cin >> quit;
					break;				
				}

				// Check for lose condition (i.e. if number of attempts reaches 0)
				if (hangman->GetAttemptsLeft() == 0) {
					// PUT LOSING CODE HERE
					hangman->Print("Drawings.txt");
					cout << endl;
					cout << "You Lose!" << endl;
					cout << "The word was " << hangman->GetWordToGuess() << "." << endl;
					
					string quit;
					cout << "Enter any key to return to the main menu: ";
					cin >> quit;
					
					break;
				}
			}
			ClearTextFile("SaveFile.txt");
		}
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "An error has occurred." << endl;
	}

	return 0;
}

// function definitions

// Takes the name of a text file
// Opens it and prints out its contents into console
void PrintTextFile(string textFileName) {
	ifstream inStream;
	inStream.open(textFileName);
	
	if (!inStream.is_open())
		throw runtime_error("File not Found");


	string line;
	while (getline(inStream, line)) {
		cout << line << endl;
	}
	inStream.close();
}

// Takes an array of strings and the size of the array
// Requests for an input from the user until they enter one of elements in the array
string RequestForValidChoice(string *validChoices, int size) {
	string user_input;
	bool isValid = false;

	while (!isValid) {
		cout << "Enter Choice: ";
		cin >> user_input;

		if (user_input.length() > 1) {

		}

		for (int i = 0; i < size; i++) {
			if (user_input == validChoices[i]) {
				isValid = true;
				break;
			}
		}
		if (!isValid) {
			cout << "Your choice is invalid. The following are valid choices: " << endl;
			for (int i = 0; i < size; i++) {
				cout << validChoices[i] << " ";
			}
			cout << endl << "Please try again." << endl;
		}
	}

	return user_input;
}

// Takes a dictionary text file name and the amount of words/lines it contains
// Selects a random line inside a text file and returns the word at the randomly selected line
string GetRandomWord(string textFileName, int totalWords) {

	int randomNumber = (rand() % totalWords) + 1;
	
	ifstream inStream;
	inStream.open(textFileName);

	if (!inStream.is_open())
		throw runtime_error("File not Found");


	string chosenWord;
	for (int i = 1; i <= randomNumber; i++) {
		getline(inStream, chosenWord);
	}
	inStream.close();

	return chosenWord;
}

// Takes a game object and the string name of the save file
// Saves the game by writing values inside the game object into the text file
void SaveGame(Game *gameObj, string nameOfSaveFile) {
	ofstream oStream;
	oStream.open(nameOfSaveFile);

	if (!oStream.is_open())
		throw runtime_error("File not Found");
	
	oStream 
		<< gameObj->GetAttemptsLeft()						<< "\n"
		<< gameObj->GetProgressWord()						<< "\n"
		<< gameObj->GetSizeOfAvailableSelections()			<< "\n"
		<< gameObj->GetWordToGuess()						<< "\n"
		<< gameObj->GetDifficulty().GetMaxAmountOfLetters()	<< "\n"
		<< gameObj->GetDifficulty().GetMinAmountOfLetters()	<< "\n"
		<< gameObj->GetDifficulty().GetNumberOfAttempts()	<< "\n";
	for (int i = 0; i < gameObj->GetSizeOfAvailableSelections(); i++) {
		oStream << gameObj->GetAvailableSelections()[i] << " ";
	}

	oStream.close();
}

// Takes the name of a text file, opens that file, and clears its contents
void ClearTextFile(string filename) {
	ofstream oStream;
	oStream.open(filename);

	if (!oStream.is_open())
		throw runtime_error("File not Found");

	oStream.close();
}

// Loads the given game object using data from the given text file
// Loading is done by replacing variables in given game object with the values saved in the text file
// returns true when something loads, otherwise returns false when save data is not found
bool LoadGame(Game *gameObj, string nameOfSaveFile) {
	ifstream inStream;
	inStream.open(nameOfSaveFile);

	if (!inStream.is_open())
		throw runtime_error("File not Found");
	
	const int numberOfLines = 8;

	string line;
	getline(inStream, line);
	if (line == "")
		return false;

	gameObj->SetAttemptsLeft(stoi(line)); //line 1
		
	getline(inStream, line);  //line 2
	gameObj->SetProgressWord(line);
		
	getline(inStream, line); //line 3
	gameObj->SetSizeOfAvailableSelections(stoi(line));

	getline(inStream, line); //line 4
	gameObj->SetWordToGuess(line);

	getline(inStream, line); //line 5
	gameObj->GetDifficulty().SetMaxAmountOfLetters(stoi(line));
		
	getline(inStream, line); //line 6
	gameObj->GetDifficulty().SetMinAmountOfLetters(stoi(line));

	getline(inStream, line); //line 7
	gameObj->GetDifficulty().SetNumberOfAttempts(stoi(line));

	getline(inStream, line); //line 8 holds the array with each element separated with spaces
	
	// Creates a temp array with the line's contents and then the availableSelections array is set to the temp array
	int index = 0;
	string* temp = new string[gameObj->GetSizeOfAvailableSelections()];
	for (int i = 0; i < line.length(); i++) { // loop will fill temp array with each letter/symbol in the line
		if (line[i] == ' ') {
			index++;
		}
		else { 
			temp[index] = line[i];
		}
	}
	gameObj->SetAvailableSelections(temp, gameObj->GetSizeOfAvailableSelections()); //temp array
	delete[] temp;

	return true;
}