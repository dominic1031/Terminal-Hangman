#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <fstream>
#include <string>
using namespace std;

class Difficulty {
public:
	Difficulty();
	Difficulty(int min, int max, int numOfAttempts);
	int GetMinAmountOfLetters() const;
	int GetMaxAmountOfLetters() const;
	int GetNumberOfAttempts() const;
	void SetMinAmountOfLetters(int min);
	void SetMaxAmountOfLetters(int max);
	void SetNumberOfAttempts(int numOfAttempts);
	Difficulty(const Difficulty &other);
	Difficulty operator=(const Difficulty &rhs);

private:
	int minAmountOfLetters;
	int maxAmountOfLetters;
	int numberOfAttempts;
};

#endif