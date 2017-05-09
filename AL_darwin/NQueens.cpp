#include "NQueens.h"



NQueens::NQueens()
{
}


NQueens::~NQueens()
{
}

Queenome NQueens::Solve(int boardSize, int populationSize, int iterationLimit)
{
	Queenome result;
	Queenome* population = new Queenome[populationSize];
	int parents = int(populationSize * 0.1);
	int children = populationSize - parents;

	for (int i = 0; i < parents; ++i)
	{
		population[i] = getRandomQueenome(8, boardSize);
	}

	for (int i = 0; i < iterationLimit; ++i)
	{
		result = i_genetics(population, parents, children, boardSize);
	}

	return result;
}

int NQueens::Fitness(const Queenome & q)
{
	int collisions = 0;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (q.data[i] == q.data[j] ||
				(q.data[i] == q.data[j] - i + j && q.data[i] - i + j < 8) || 
				(q.data[i] == q.data[j] - i - j && q.data[i] - i - j >= 0))
				collisions++;
		}
	}

	return collisions;
}

Queenome NQueens::getRandomQueenome(uint limit, uint boardSize)
{
	Queenome q;

	for (uint i = 0; i < boardSize; ++i)
	{
		q.data[i] = int(m_rng.GetZeroToOne() * limit);
	}

	return q;
}

Queenome NQueens::i_genetics(Queenome * population, int parents, int children, int boardSize)
{
	Queenome result, tmpQueenome;

	for (int i = parents; i < children + parents; ++i)
	{
		int r = parents * m_rng.GetZeroToOne();
		tmpQueenome = population[r];
		r = parents * m_rng.GetZeroToOne();
		population[i] = i_combine(tmpQueenome, population[r]);
		population[i].fitness = Fitness(population[i]);
	}

	return result;
}

Queenome NQueens::i_combine(const Queenome & qLeft, const Queenome & qRight)
{
	Queenome result;
	int r = m_rng.GetZeroToOne() * 8;

	memcpy(result.data, qLeft.data, r * sizeof(int));
	memcpy(&(result.data[r]), &(qRight.data[r]), (8 - r) * sizeof(int));

	return result;
}
