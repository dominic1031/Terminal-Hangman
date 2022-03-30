#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

Game::Game() {
	wordToGuess = "";
	difficulty = Difficulty(1, 50, 5);
	progressWord = "";
	attemptsLeft = difficulty.GetNumberOfAttempts();
	sizeOfAvailableSelections = 28;
	availableSelections = new string[sizeOfAvailableSelections];
	
	// converts chars with ASCII values 97 to 122 (a to z) to strings and puts it in array
	for (int i = 0; i < sizeOfAvailableSelections-1; i++) {
		availableSelections[i] = string(1, (i + 97)); 
	}
	availableSelections[26] = "?";
	availableSelections[27] = "*";

}

// Takes a letterToAdd and updates the progressWord to show that letter
// For example, if letterToAdd = "a", progressWord = "-----", and wordToGuess = "apple", then progressWord will update to "a----" 
void Game::UpdateProgressWord(string letterToAdd) {

	for (int i = 0; i < wordToGuess.length(); i++) {
		if (wordToGuess[i] == letterToAdd[0]) {
			progressWord[i] = letterToAdd[0];
		}
	}
}

// Print the current hangman drawing based on attempts left
void Game::Print(string drawingsFileName) {
	ifstream drawings;
	drawings.open(drawingsFileName);

	if (!drawings.is_open())
		throw runtime_error("File Not Found!");

	// the drawings have a series of frames corresponding to how many attempts left
	int const totalFrames = 11;
	int const frameHeight = 8;
	int frameNumber = totalFrames - attemptsLeft; // 1st frame = 1; 2nd frame = 2; etc. up to 11 frames
	int frameStartingLineNumber = frameNumber * frameHeight - frameHeight + 1; //where the frame begins in the file
	int frameEndingLineNumber = frameNumber * frameHeight; // where the frame ends in the file
	string line;
	for (int i = 1; i <= frameEndingLineNumber; i++) {
		getline(drawings, line);
		if (frameStartingLineNumber <= i && i <= frameEndingLineNumber)
			cout << line << endl;
	}
	drawings.close();
}

//Getter and Setters
Difficulty Game::GetDifficulty() { return difficulty; }
string Game::GetWordToGuess() { return wordToGuess; }
string Game::GetProgressWord() { return progressWord; }
int Game::GetAttemptsLeft() { return attemptsLeft; }
string* Game::GetAvailableSelections() { return availableSelections; }
int Game::GetSizeOfAvailableSelections() { return sizeOfAvailableSelections; }
void Game::SetDifficulty(Difficulty d) { difficulty = d; }
void Game::SetWordToGuess(string word) { wordToGuess = word; }
void Game::SetProgressWord(string word) { progressWord = word; }
void Game::SetAttemptsLeft(int num) { attemptsLeft = num; }
void Game::SetAvailableSelections(string* arr, int size) {
	for (int i = 0; i < size; i++) {
		availableSelections[i] = arr[i];
	}
	for (int i = sizeOfAvailableSelections - 1; i >= size; i--) {
		availableSelections[i] = "";
	}
	sizeOfAvailableSelections = size;
}
void Game::SetSizeOfAvailableSelections(int size) { sizeOfAvailableSelections = size; }

Game::~Game() {
	delete[] availableSelections;
}