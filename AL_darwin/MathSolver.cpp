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
	m_limit(100),
	m_exact(PLUS),
	m_inherit(NONE)
{
}

MathSolver::~MathSolver()
{
}

void MathSolver::setMode(MODE m)
{
	m_mode = m;
}

void MathSolver::setExactly(EXACTLY e)
{
	m_exact = e;
}

void MathSolver::setInheritance(INHERIT i)
{
	m_inherit = i;
}

int MathSolver::Solve(Genome* output)
{
	Genome result;
	Genome* population = nullptr;

	bool solution = false;
	int mutateParam = m_limit;
	int cnt = 0;
	for(int i = 0; i < m_iterationLimit; ++i)
	{
		/*if (mutateParam > 10)
			mutateParam /= 2;
		else*/
			mutateParam = 5;

		if (m_mode == ONEPLUSONE)
		{
			if (i == 0)
				result = getRandomGenome(m_limit);
			result = i_onePlusOne(result, mutateParam);
		}
		else if (m_mode == MULAMBDA)
		{
			if (i == 0)
			{
				population = new Genome[m_mu + m_lambda];

				for (uint j = 0; j < m_mu; ++j)
				{
					population[j] = getRandomGenome(m_limit);
					population[j].fitness = Fitness(population[i]);
				}
			}

			//int mutateParam = std::round(float(m_limit) *(1.0 - (i / m_iterationLimit)));
			//if(m_mode == MUPLUSLAMBDA)
			//	i_muPlusLambda(population, mutateParam);
			//else
				i_muLambda(population, mutateParam);

			result = population[0];

			if (i > 9990 && (result.fitness > 0 && result.fitness < 2))
				continue;
		}

		if (result.fitness == 0)
		{
			solution = true;
			cnt = i;
			break;
		}
	}

	if (m_debug && solution)
		std::cout << "solution found in " << cnt << " steps!\n";
	else if(m_debug)
		std::cout << "impossibru!\n";

	delete[] population;

	//set return values
	*output = result;
	return cnt;
}

int MathSolver::Fitness(Genome& g)
{
	if (g.a == 0 && g.b == 0 && g.x == 0 && g.y == 0)
		return INT_MAX;

	//3x� + 5y� = 7a + 3b�
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
	//uint half;
	//half = limit / 2;

	g.x = int(m_rng.GetZeroToOne() * limit * 2 - limit);
	g.y = int(m_rng.GetZeroToOne() * limit * 2 - limit);
	g.a = int(m_rng.GetZeroToOne() * limit * 2 - limit);
	g.b = int(m_rng.GetZeroToOne() * limit * 2 - limit);

	return g;
}

Genome MathSolver::i_onePlusOne(const Genome& g, int mutateParam)
{
	Genome child[2] = { g, g };

	for (int i = 0; i < 2; ++i)
	{
		child[i] = i_mutateWith(child[i], mutateParam);

		if(child[i].a > child[i].b)	//solution requirement: a > b 
			child[i].fitness = Fitness(child[i]);
	}

	if (child[0].fitness < child[1].fitness)
		return child[0];
	else
		return child[1];
}

//void MathSolver::i_muPlusLambda(Genome* g, int mutateParam)
//{
//	for (uint i = m_mu; i < m_mu + m_lambda; ++i)
//	{
//		//uint r = int(m_mu * m_rng.GetZeroToOne());
//		uint r = i%m_mu;
//		g[i] = i_mutateWith(g[r], mutateParam);
//	}
//
//	for (uint i = 0; i < m_mu + m_lambda; ++i)
//	{
//		if(g[i].a > g[i].b)
//			g[i].fitness = Fitness(g[i]);
//	}
//
//	std::qsort(g, m_mu + m_lambda, sizeof(Genome), Math::GenomeCompare);
//}

void MathSolver::i_muLambda(Genome * population, int mutateParam)
{
	Genome tmpGenome;
	for (uint i = m_mu; i < m_mu + m_lambda; ++i)
	{
		uint r = int(m_mu * m_rng.GetZeroToOne());

		if (m_exact == NONE)
			tmpGenome = population[r];
		else if (m_exact == COMBINE)
		{
			tmpGenome = population[r];
			r = int(m_mu * m_rng.GetZeroToOne());
			tmpGenome = i_combine(tmpGenome, population[r]);
		}
		else if (m_exact == BLEND)
		{
			tmpGenome = population[r];
			r = int(m_mu * m_rng.GetZeroToOne());
			tmpGenome = i_blend(tmpGenome, population[r]);
		}

		tmpGenome = i_mutateWith(tmpGenome, mutateParam);

		if (tmpGenome.a >tmpGenome.b)
			tmpGenome.fitness = Fitness(tmpGenome);

		population[i] = tmpGenome;
	}

	int offset = 0;
	if (m_exact == COMMA)
		offset = m_mu;

	std::qsort(population + offset, m_mu + m_lambda - offset, sizeof(Genome), Math::GenomeCompare);

	if (m_exact == COMMA)
		memcpy(population, population + offset, sizeof(Genome) * m_mu);
}

Genome MathSolver::i_mutate(Genome& g)
{
	return i_mutateWith(g, 10);
}

Genome MathSolver::i_mutateWith(Genome & g, int margin)
{
	Genome gMutated = g;

	gMutated.x += /*std::round(m_rng.GetZeroToOne())**/(m_rng.GetZeroToOne() * margin * 2 - margin);
	gMutated.y += /*std::round(m_rng.GetZeroToOne())**/(m_rng.GetZeroToOne() * margin * 2 - margin);
	gMutated.a += /*std::round(m_rng.GetZeroToOne())**/(m_rng.GetZeroToOne() * margin * 2 - margin);
	gMutated.b += /*std::round(m_rng.GetZeroToOne())**/(m_rng.GetZeroToOne() * margin * 2 - margin);

	return gMutated;
}

Genome MathSolver::i_combine(const Genome & gLeft, const Genome & gRight)
{
	Genome result;

	result.x = (m_rng.GetZeroToOne() > 0.5f) ? gLeft.x : gRight.x;
	result.y = (m_rng.GetZeroToOne() > 0.5f) ? gLeft.y : gRight.y;
	result.a = (m_rng.GetZeroToOne() > 0.5f) ? gLeft.a : gRight.a;
	result.b = (m_rng.GetZeroToOne() > 0.5f) ? gLeft.b : gRight.b;

	return result;
}

Genome MathSolver::i_blend(const Genome & gLeft, const Genome & gRight)
{
	Genome result;

	result.x = int((gLeft.x + gRight.x) * 0.5);
	result.y = int((gLeft.y + gRight.y) * 0.5);
	result.a = int((gLeft.a + gRight.a) * 0.5);
	result.b = int((gLeft.b + gRight.b) * 0.5);

	return result;
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
	if ((*(Genome*)lhs).fitness <  (*(Genome*)rhs).fitness)
		return -1;
	if ((*(Genome*)lhs).fitness == (*(Genome*)rhs).fitness)
		return 0;
	if ((*(Genome*)lhs).fitness >  (*(Genome*)rhs).fitness)
		return 1;
}
