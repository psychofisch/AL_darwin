#include "MathSolver.h"

MathSolver::MathSolver()
	:MathSolver(MODE::ONEPLUSONE)
{
}

MathSolver::MathSolver(MODE m)
	:m_mode(m),
	m_iterationLimit(10000)
{
}

MathSolver::~MathSolver()
{
}

void MathSolver::setMode(MODE m)
{
	m_mode = m;
}

Genome MathSolver::Solve(Genome& genome)
{
	Genome result = genome;

	bool solution = false;
	int cnt = 0;
	for(int i = 0; i < m_iterationLimit; ++i)
	{
		switch (m_mode)
		{
		case ONEPLUSONE:
			result = i_onePlusOne(result);
			break;
		/*case MUCOMMALAMDA:
			result = i_muPlusLambda(result);
			break;*/
		default:
			break;
		}

		int fitness = Fitness(result);
		if (fitness == 0)
		{
			solution = true;
			cnt = i;
			break;
		}
	}

	if (solution)
		std::cout << "solution found in " << cnt << " steps!\n";
	else
		std::cout << "impossibru!\n";

	return result;
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

Genome MathSolver::i_onePlusOne(Genome& g)
{
	Genome child[2] = { g, g };

	for (int i = 0; i < 2; ++i)
	{
		child[i] = i_mutate(child[i]);

		if(child[i].a >= child[i].b)	//solution requirement: a > b 
			child[i].fitness = Fitness(child[i]);
	}

	if (child[0].fitness < child[1].fitness)
		return child[0];
	else
		return child[1];
}

Genome* MathSolver::i_muPlusLambda(Genome* g, uint muParents, uint lambdaChildren)
{
	Genome* child = new Genome[lambdaChildren + muParents];

	for (int i = 0; i < lambdaChildren; ++i)
	{
		unsigned int r = muParents * m_rng.GetZeroToOne();
		child[i] = i_mutate(g[r]);
	}

	std::memcpy(child + lambdaChildren, g, muParents);

	return g;
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
