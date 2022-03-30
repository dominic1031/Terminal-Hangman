#pragma once
#ifndef GAME_H
#define GAME_H
#include "Difficulty.h"

class Game {
public:
	Game();
	void UpdateProgressWord(string letterToShow);
	void Print(string drawingsFileName); 
	Difficulty GetDifficulty();
	string GetWordToGuess();
	string GetProgressWord();
	int GetAttemptsLeft();
	string* GetAvailableSelections();
	int GetSizeOfAvailableSelections();
	void SetDifficulty(Difficulty d);
	void SetWordToGuess(string word);
	void SetProgressWord(string word);
	void SetAttemptsLeft(int num);
	void SetAvailableSelections(string* arr, int size);
	void SetSizeOfAvailableSelections(int size);
	~Game();
private:
	Difficulty difficulty;
	string wordToGuess; // the word that the player will be guessing
	string progressWord; // word that indicates what the player knows so far. EX: "a--le" for "apple" with 'a', 'l', and 'e' guessed
	int attemptsLeft; 
	string *availableSelections; // list of what are valid inputs during the game
	int sizeOfAvailableSelections; // indicates the array size of availableSelections
};
#endif