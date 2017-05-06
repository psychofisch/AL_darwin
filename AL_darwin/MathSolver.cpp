#include "MathSolver.h"

MathSolver::MathSolver()
	:MathSolver(MODE::ONEPLUSONE)
{
}

MathSolver::MathSolver(MODE m)
	:m_mode(m),
	m_iterationLimit(10000),
	m_debug(false),
	m_mu(10),
	m_lambda(10),
	m_limit(100)
{
}

MathSolver::~MathSolver()
{
}

void MathSolver::setMode(MODE m)
{
	m_mode = m;
}

int MathSolver::Solve(Genome* output)
{
	Genome result;
	Genome* population = nullptr;

	bool solution = false;
	int cnt = 0;
	for(int i = 0; i < m_iterationLimit; ++i)
	{
		if (m_mode == ONEPLUSONE)
		{
			if (i == 0)
				result = getRandomGenome(m_limit);
			result = i_onePlusOne(result);
		}
		else if (m_mode == MUPLUSLAMBDA)
		{
			if (i == 0)
			{
				population = new Genome[m_mu + m_lambda];

				for (uint i = 0; i < m_mu; ++i)
				{
					population[i] = getRandomGenome(m_limit);
				}
			}

			i_muPlusLambda(population);

			result = population[0];
		}

		if (result.fitness == 0)
		{
			solution = true;
			cnt = i;
			break;
		}
	}

	delete[] population;

	if (m_debug && solution)
		std::cout << "solution found in " << cnt << " steps!\n";
	else if(m_debug)
		std::cout << "impossibru!\n";

	//set return values
	*output = result;
	return cnt;
}

int MathSolver::Fitness(Genome& g)
{
	if (g.a == 0 && g.b == 0 && g.x == 0 && g.y == 0)
		return INT_MAX;

	//3x² + 5y³ = 7a + 3b²
	int result;

	result = ((3 * Math::powi(g.x, 2)) + (5 * Math::powi(g.y, 3))) - ((7 * g.a) + (3 * Math::powi(g.b, 2)));

	return std::abs(result);
}

void MathSolver::setSeed(const unsigned long seed)
{
	m_rng.seed(seed);
}

void MathSolver::setMu(uint m)
{
	m_mu = m;
}

void MathSolver::setLambda(uint l)
{
	m_lambda = l;
}

void MathSolver::setDebug(bool d)
{
	m_debug = d;
}

void MathSolver::setLimit(uint l)
{
	m_limit = l;
}

Genome MathSolver::getRandomGenome(uint limit)
{
	Genome g;
	uint half;
	half = limit / 2;

	g.x = int(m_rng.GetZeroToOne() * limit - half);
	g.y = int(m_rng.GetZeroToOne() * limit - half);
	g.a = int(m_rng.GetZeroToOne() * limit - half);
	g.b = int(m_rng.GetZeroToOne() * limit - half);

	return g;
}

Genome MathSolver::i_onePlusOne(Genome& g)
{
	Genome child[2] = { g, g };

	for (int i = 0; i < 2; ++i)
	{
		child[i] = i_mutate(child[i]);

		if(child[i].a > child[i].b)	//solution requirement: a > b 
			child[i].fitness = Fitness(child[i]);
	}

	if (child[0].fitness < child[1].fitness)
		return child[0];
	else
		return child[1];
}

void MathSolver::i_muPlusLambda(Genome* g)
{
	for (uint i = m_mu; i < m_mu + m_lambda; ++i)
	{
		//uint r = int(m_mu * m_rng.GetZeroToOne());
		uint r = i%m_mu;
		g[i] = i_mutate(g[r]);
	}

	for (uint i = 0; i < m_mu + m_lambda; ++i)
	{
		if(g[i].a > g[i].b)
			g[i].fitness = Fitness(g[i]);
	}

	std::qsort(g, m_mu + m_lambda, sizeof(Genome), Math::GenomeCompare);
}

Genome MathSolver::i_mutate(Genome& g)
{
	return i_mutateWith(g, 10);
}

Genome MathSolver::i_mutateWith(Genome & g, uint margin)
{
	Genome gMutated = g;

	unsigned int half = margin / 2;
	gMutated.x += std::round(m_rng.GetZeroToOne())*(m_rng.GetZeroToOne() * margin - half);
	gMutated.y += std::round(m_rng.GetZeroToOne())*(m_rng.GetZeroToOne() * margin - half);
	gMutated.a += std::round(m_rng.GetZeroToOne())*(m_rng.GetZeroToOne() * margin - half);
	gMutated.b += std::round(m_rng.GetZeroToOne())*(m_rng.GetZeroToOne() * margin - half);

	return gMutated;
}

int Math::powi(int base, uint exponent)
{
	int result = base;
	for (unsigned int i = 1; i < exponent; ++i)
		result *= base;
	return result;
}

int Math::GenomeCompare(const void * lhs, const void * rhs)
{
	if ((*(Genome*)lhs).fitness < (*(Genome*)rhs).fitness)
		return -1;
	if ((*(Genome*)lhs).fitness == (*(Genome*)rhs).fitness)
		return 0;
	if ((*(Genome*)lhs).fitness >(*(Genome*)rhs).fitness)
		return 1;
}
