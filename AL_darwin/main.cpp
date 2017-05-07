#include <iostream>
#include <ctime>

#include "MathSolver.h"
#include "Stopwatch.h"

std::ostream& operator<<(std::ostream& os, const Genome& g) {
	return os	/*<< "x: "*/ << g.x << ","
		/*<< "y: "*/ << g.y << ","
		/*<< "a: "*/ << g.a << ","
		/*<< "b:" */ << g.b;
}

void testing(MathSolver& ms, int sampleSize, Genome& result)
{
	Stopwatch watch;
	watch.start();
	long steps = 0;
	int lostRate = 0;
	for (int i = 0; i < sampleSize; ++i)
	{
		//ms.setSeed(rng.GetNumber());
		int tmpSteps = ms.Solve(&result);
		if (result.fitness != 0)
		{
			lostRate++;
		}
		else
			steps += tmpSteps;
	}
	watch.stop();

	std::cout << "average steps: " << ((steps > 0)?(steps / (sampleSize - lostRate)):steps) << std::endl;
	std::cout << "time: " << watch.getDurationString(0) << std::endl;
	std::cout << "lostrate: " << float(lostRate) / sampleSize * 100.f << "%" << std::endl;
	std::cout << "*******\n";
}

int main(int argc, char* argv[])
{
	MathSolver ms;
	ms.setDebug(false);
	ms.setLimit(100);

	RNGesus rng;
	int sampleSize = 100;
	Genome result;
	Stopwatch watch;

	while (1)
	{
		watch = Stopwatch();
		//ms.setSeed(time(NULL));
		ms.setSeed(2);

		/*std::cout << "*******\n1+1\n";
		watch.start();
		ms.setMode(MathSolver::ONEPLUSONE);
		testing(ms, sampleSize, result);

		std::cout << "*******\n1+1 with Mu+Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::PLUS);
		ms.setMu(1);
		ms.setLambda(1);
		testing(ms, sampleSize, result);

		std::cout << "*******\nMu+Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::PLUS);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);*/
		std::cout << "samplesize: " << sampleSize << std::endl;

		std::cout << "*******\nMu,Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);

		std::cout << "*******\nMu,Lambda COMBINE\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::COMBINE);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);

		std::cout << "*******\nMu,Lambda BLEND\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::BLEND);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);

		std::cin.ignore();
	}
}
