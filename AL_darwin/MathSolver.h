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

	Genome(const Genome& g)
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
	enum MODE { ONEPLUSONE = 0, MULAMBDA };
	enum EXACTLY { PLUS = 0, COMMA };
	enum INHERIT { NONE = 0, COMBINE, BLEND };

	MathSolver();
	MathSolver(MODE m);
	~MathSolver();

	void setMode(MODE m);
	void setExactly(EXACTLY e);
	void setInheritance(INHERIT i);
	int Solve(Genome* output);
	int Fitness(const Genome& g1);
	void setSeed(const unsigned long seed);
	void setMu(uint m);
	void setLambda(uint l);
	void setDebug(bool d);
	void setLimit(uint l);

	Genome getRandomGenome(uint limit);

	int* qualityCourse;

private:
	MODE m_mode;
	EXACTLY m_exact;
	INHERIT m_inherit;
	RNGesus m_rng;
	uint	m_iterationLimit,
			m_mu,
			m_lambda,
			m_limit;
	bool m_debug;

	Genome i_onePlusOne(const Genome& g, int mutateParam);
	//void i_muPlusLambda(Genome* g, int mutateParam);
	void i_muLambda(Genome* g, int mutateParam);
	Genome i_mutate(Genome& g);
	Genome i_mutateWith(Genome& g, int margin);
	Genome i_combine(const Genome& gLeft, const Genome& gRight);
	Genome i_blend(const Genome& gLeft, const Genome& gRight);
};

