#include <iostream>
#include <ctime>

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
	ms.setDebug(false);
	//std::cout << "FitnessTest -> " << ms.Fitness(Genome(2, -1, 1, 0)) << std::endl;

	RNGesus rng;
	/*int a = 0;
	for (int i = 0; i < 10000; ++i)
	{
		if (rng.GetZeroToOne() > 0.9999f)
			a++;
	}
	std::cout << a << ":" << 1000-a << std::endl;*/

	int sampleSize = 10;
	Genome result;

	while (1)
	{
		ms.setSeed(time(NULL));

		ms.setMode(MathSolver::ONEPLUSONE);
		long steps = 0;
		for (int i = 0; i < sampleSize; ++i)
		{
			//ms.setSeed(rng.GetNumber());
			steps += ms.Solve(&result);
		}

		if (result.fitness == 0)
			std::cout << "ONEPLUSONE average steps: " << steps / sampleSize << std::endl;
		else
			std::cout << "ONEPLUSONE couldn't find any solution..." << std::endl;

		ms.setMode(MathSolver::MUPLUSLAMBDA);
		ms.setMu(10);
		ms.setLambda(20);
		steps = 0;
		for (int i = 0; i < sampleSize; ++i)
		{
			//ms.setSeed(rng.GetNumber());
			steps += ms.Solve(&result);
		}

		if (result.fitness == 0)
			std::cout << "MUPLUSLAMBDA average steps: " << steps / sampleSize << std::endl;
		else
			std::cout << "MUPLUSLAMBDA couldn't find any solution..." << std::endl;

		ms.setMu(20);
		ms.setLambda(10);
		steps = 0;
		for (int i = 0; i < sampleSize; ++i)
		{
			//ms.setSeed(rng.GetNumber());
			steps += ms.Solve(&result);
		}

		if (result.fitness == 0)
			std::cout << "MUPLUSLAMBDA average steps: " << steps / sampleSize << std::endl;
		else
			std::cout << "MUPLUSLAMBDA couldn't find any solution..." << std::endl;

		std::cin.ignore();
	}
}
