#pragma once

#include <iostream>
#include <vector>

#include "RNGesus.h"

typedef unsigned int uint;

struct Queenome {
	std::vector<int> data;
	int fitness = INT_MAX;
};

namespace Kingdom {
	int QueenCompare(const void* lhs, const void* rhs);
	void printQueenome(const Queenome& q);
}

class NQueens
{
public:
	NQueens();
	~NQueens();

	int Solve(int boardSize, int populationSize, int iterationLimit, Queenome* output);
	int Fitness(const Queenome& q);
	void setSeed(const unsigned long s);

	Queenome getRandomQueenome(uint limit, uint boardSize);

private:
	RNGesus m_rng;

	void i_genetics(Queenome* population, int parents, int children, int boardSize);
	Queenome i_combine(const Queenome& qLeft, const Queenome& qRight);
	Queenome i_mutate(Queenome& g, int margin);
};

