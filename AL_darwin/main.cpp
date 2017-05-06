#include <iostream>

#include "MathSolver.h"

std::ostream& operator<<(std::ostream& os, const Genome& g) {
	return os	/*<< "x: "*/ << g.x << ","
		/*<< "y: "*/ << g.y << ","
		/*<< "a: "*/ << g.a << ","
		/*<< "b:" */ << g.b;
}

int main(int argc, char* argv[])
{
	MathSolver ms;
	ms.setMode(MathSolver::MUPLUSLAMBDA);
	//std::cout << "FitnessTest -> " << ms.Fitness(Genome(2, -1, 1, 0)) << std::endl;

	RNGesus rng;
	/*int a = 0;
	for (int i = 0; i < 10000; ++i)
	{
		if (rng.GetZeroToOne() > 0.9999f)
			a++;
	}
	std::cout << a << ":" << 1000-a << std::endl;*/

	for (int i = 0; i < 10; ++i)
	{
		ms.setSeed(rng.GetNumber());

		Genome testGene, result;
		unsigned int margin = 1000,
			half;
		half = margin / 2;

		testGene.b = rng.GetZeroToOne() * margin - half;
		testGene.x = rng.GetZeroToOne() * margin - half;
		testGene.y = rng.GetZeroToOne() * margin - half;
		testGene.b = rng.GetZeroToOne() * margin - half;

		std::cout << "#####\n" << testGene << std::endl;

		result = ms.Solve(testGene);

		std::cout << result << "\n#####" << std::endl;
	}

	std::cin.ignore();
}
