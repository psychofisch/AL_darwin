#include "NQueens.h"



NQueens::NQueens():
	m_debug(false)
{
}


NQueens::~NQueens()
{
}

int NQueens::Solve(int boardSize, int populationSize, int iterationLimit, Queenome* output)
{
	Queenome result;
	Queenome* population = new Queenome[populationSize];
	int parents = int(populationSize * 0.1);
	int children = populationSize - parents;

	bool solution = false;
	int cnt = 0,
		extinctions = 0;
	for (int i = 0; i < iterationLimit; ++i)
	{
		if (i % int(iterationLimit * 0.2) == 0)
		{
			extinctions++;
			for (int i = 0; i < parents; ++i)
			{
				population[i] = getRandomQueenome(boardSize, boardSize);
				population[i].fitness = Fitness(population[i]);
			}
		}

		i_genetics(population, parents, children, boardSize);
		result = population[0];

		if (result.data[0] < 0 || result.data[boardSize - 1] > boardSize)
			continue;

		if (result.fitness == 0)
		{
			solution = true;
			cnt = i;
			break;
		}
	}

	if (m_debug)
	{
		if (solution)
			std::cout << "solution found in " << cnt << " steps!\n";
		else
			std::cout << "impossibru!\n";
		std::cout << "extinctions: " << extinctions << std::endl;
	}

	delete[] population;

	if(output != nullptr)
		*output = result;
	return cnt;
}

int NQueens::Fitness(const Queenome & q)
{
	int collisions = 0;
	size_t s = q.data.size();

	for (int i = 0; i < s; ++i)
	{
		for (int j = i + 1; j < s; ++j)
		{
			int newIndex = -i + j;
			if (newIndex < 0)
				continue;

			if (q.data[i] == q.data[j]
				|| (q.data[i] == q.data[j] - i + j)
				|| (q.data[i] == q.data[j] + i - j)
				)
				collisions++;
		}
	}

	return collisions;
}

void NQueens::setSeed(const unsigned long s)
{
	m_rng.seed(s);
}

void NQueens::setDebug(bool d)
{
	m_debug = d;
}

Queenome NQueens::getRandomQueenome(uint limit, uint boardSize)
{
	Queenome q;
	q.data.resize(boardSize);

	for (uint i = 0; i < boardSize; ++i)
	{
		q.data[i] = int(m_rng.GetZeroToOne() * limit);
	}

	return q;
}

void NQueens::i_genetics(Queenome * population, int parents, int children, int boardSize)
{
	Queenome tmpQueenome;

	for (int i = parents; i < children + parents; ++i)
	{
		int r = parents * m_rng.GetZeroToOne();
		tmpQueenome = population[r];
		r = parents * m_rng.GetZeroToOne();
		population[i] = i_combine(tmpQueenome, population[r]);
		population[i] = i_mutate(population[i], population[0].data.size());
		population[i].fitness = Fitness(population[i]);
	}

	std::qsort(population + parents, children, sizeof(Queenome), Kingdom::QueenCompare);

	//memcpy(population, population + parents, sizeof(Queenome) * parents);

	for (int i = 0; i < parents; ++i)
	{
		population[i] = population[parents + i];
	}
}

Queenome NQueens::i_combine(const Queenome & qLeft, const Queenome & qRight)
{
	Queenome result = qLeft;
	int r = m_rng.GetZeroToOne() * qLeft.data.size();

	//memcpy(&result.data[0], &qLeft.data[0], r * sizeof(int));
	memcpy(&(result.data[r]), &(qRight.data[r]), (qLeft.data.size() - r) * sizeof(int));

	return result;
}

Queenome NQueens::i_mutate(Queenome & g, int margin)
{
	Queenome q = g;
	size_t s = g.data.size();
	int r = int(m_rng.GetZeroToOne() * s);
	q.data[r] += int(m_rng.GetZeroToOne() * margin * 2 - margin);

	if (q.data[r] < 0)
		q.data[r] += s;
	else if (q.data[r] > s - 1)
		q.data[r] -= s;

	/*for (uint i = 0; i < s; ++i)
	{
		q.data[i] += int(m_rng.GetZeroToOne() * margin * 2 - margin);

		if (q.data[i] < 0)
			q.data[i] += s;
		else if (q.data[i] > s - 1)
			q.data[i] %= s;
	}*/

	return q;
}

int Kingdom::QueenCompare(const void * lhs, const void * rhs)
{
	if ((*(Queenome*)lhs).fitness <  (*(Queenome*)rhs).fitness)
		return -1;
	if ((*(Queenome*)lhs).fitness == (*(Queenome*)rhs).fitness)
		return 0;
	if ((*(Queenome*)lhs).fitness >(*(Queenome*)rhs).fitness)
		return 1;
}

void Kingdom::printQueenome(const Queenome& q)
{
	for (int i = 0; i < q.data.size(); ++i)
	{
		//if (i % 2 == 0)
			for (int j = 0; j < q.data.size(); ++j)
			{
				//if(j % 3 != 1)
				if (q.data[i] == j)
					std::cout << "D";
				else if((j % 2 == 0 && i % 2 == 0) ||
					j % 2 == 1 && i % 2 == 1)
					std::cout << "#";
				else// if (j % 2 == 1)
					std::cout << " ";
				//std::cout << " ";
			}
		std::cout << std::endl;
	}
}
