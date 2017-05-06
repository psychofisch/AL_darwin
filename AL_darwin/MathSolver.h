#pragma once

#include <iostream>

#include "RNGesus.h"

typedef unsigned int uint;

namespace Math{
	int powi(int base, uint exponent);
}

struct Genome {
	Genome(int px, int py, int pa, int pb)
	{
		x = px;
		y = py;
		a = pa;
		b = pb;
	}

	Genome(Genome& g)
		:Genome(g.x, g.y, g.a, g.b)
	{
	}

	Genome()
		:Genome(0, 0, 0, 0)
	{
	}

	int x, y, a, b;
};

class MathSolver
{
public:
	enum MODE { ONEPLUSONE = 0, MUPLUSLAMBDA, MUCOMMALAMDA };

	MathSolver();
	MathSolver(MODE m);
	~MathSolver();

	void setMode(MODE m);
	Genome Solve(Genome& genome);
	int Fitness(Genome& g1);
	void setSeed(const unsigned long seed);

private:
	MODE m_mode;
	typedef Genome(MathSolver::*MathSolverMemberFunctPtr)(Genome& g);
	MathSolverMemberFunctPtr m_functionPtr;
	RNGesus m_rng;
	int m_iterationLimit;

	Genome i_onePlusOne(Genome& g);
	Genome* i_muPlusLambda(Genome* g, uint muParents, uint lambdaChildren);
	Genome i_mutate(Genome& g);
	Genome i_mutateWith(Genome& g, uint margin);
};

