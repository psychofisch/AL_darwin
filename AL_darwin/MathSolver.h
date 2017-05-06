#pragma once

#include <iostream>

#include "RNGesus.h"

typedef unsigned int uint;

namespace Math{
	int powi(int base, uint exponent);
	int GenomeCompare(const void* lhs, const void* rhs);
}

struct Genome {
	Genome(int px, int py, int pa, int pb, int pf)
		:x(px),
		y(py),
		a(pa),
		b(pb),
		fitness(pf)
	{}

	Genome(Genome& g)
		:Genome(g.x, g.y, g.a, g.b, g.fitness)
	{}

	Genome()
		:Genome(0, 0, 0, 0, INT_MAX)
	{}

	int x, y, a, b, fitness;
};

class MathSolver
{
public:
	enum MODE { ONEPLUSONE = 0, MUPLUSLAMBDA, MUCOMMALAMDA };

	MathSolver();
	MathSolver(MODE m);
	~MathSolver();

	void setMode(MODE m);
	int Solve(Genome* output);
	int Fitness(Genome& g1);
	void setSeed(const unsigned long seed);
	void setMu(uint m);
	void setLambda(uint l);
	void setDebug(bool d);
	void setLimit(uint l);

	Genome getRandomGenome(uint limit);

private:
	MODE m_mode;
	RNGesus m_rng;
	int m_iterationLimit;
	uint m_mu, m_lambda;
	bool m_debug;
	uint m_limit;

	Genome i_onePlusOne(Genome& g, int mutateParam);
	void i_muPlusLambda(Genome* g, int mutateParam);
	void i_muCommaLambda(Genome* g, int mutateParam);
	Genome i_mutate(Genome& g);
	Genome i_mutateWith(Genome& g, int margin);
};

