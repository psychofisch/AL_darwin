#pragma once

#include <iostream>

#include "RNGesus.h"

typedef unsigned int uint;

struct Queenome {
	int data[8] = { 0 };
	int fitness;
};

class NQueens
{
public:
	NQueens();
	~NQueens();

	Queenome Solve(int boardSize, int populationSize, int iterationLimit);
	int Fitness(const Queenome& q);

	Queenome getRandomQueenome(uint limit, uint boardSize);

private:
	RNGesus m_rng;

	Queenome i_genetics(Queenome* population, int parents, int children, int boardSize);
	Queenome i_combine(const Queenome& qLeft, const Queenome& qRight);
};

