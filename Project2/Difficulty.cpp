#include "Difficulty.h"

Difficulty::Difficulty() : 
	minAmountOfLetters(1), 
	maxAmountOfLetters(100),
	numberOfAttempts(10){
}

Difficulty::Difficulty(int min, int max, int numOfAttempts):
	minAmountOfLetters(min), maxAmountOfLetters(max), numberOfAttempts(numOfAttempts){
}

int Difficulty::GetMinAmountOfLetters() const {
	return minAmountOfLetters;
}

int Difficulty::GetMaxAmountOfLetters() const {
	return maxAmountOfLetters;
}

int Difficulty::GetNumberOfAttempts() const {
	return numberOfAttempts;
}

void Difficulty::SetMinAmountOfLetters(int min) {
	minAmountOfLetters = min;
}
void Difficulty::SetMaxAmountOfLetters(int max) {
	maxAmountOfLetters = max;
}
void Difficulty::SetNumberOfAttempts(int numOfAttempts) {
	numberOfAttempts = numOfAttempts;
}

Difficulty::Difficulty(const Difficulty &other) {
	minAmountOfLetters = other.GetMinAmountOfLetters();
	maxAmountOfLetters = other.GetMaxAmountOfLetters();
	numberOfAttempts = other.GetNumberOfAttempts();
}

Difficulty Difficulty::operator=(const Difficulty &rhs) {
	minAmountOfLetters = rhs.GetMinAmountOfLetters();
	maxAmountOfLetters = rhs.GetMaxAmountOfLetters();
	numberOfAttempts = rhs.GetNumberOfAttempts();
	return *this;
}
