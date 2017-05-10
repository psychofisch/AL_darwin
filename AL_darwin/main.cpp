#include <iostream>
#include <ctime>
#include <fstream>

#include "MathSolver.h"
#include "NQueens.h"
#include "Stopwatch.h"

std::ostream& operator<<(std::ostream& os, const Genome& g) {
	return os	/*<< "x: "*/ << g.x << ","
		/*<< "y: "*/ << g.y << ","
		/*<< "a: "*/ << g.a << ","
		/*<< "b:" */ << g.b;
}

std::ostream& operator<<(std::ostream& os, const Queenome& q) {
	for (int i = 0; i < 8; ++i)
	{
		os << q.data[i];
		if (i < 7)
			os << ", ";
	}

	return os;
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

void SaveQuality(const char * path, const char* prefix, int* data)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	file << prefix << std::endl;

	for (uint i = 0; i < 10000; ++i)
	{
		if(data[i] >= 0)
			file << data[i] << std::endl;
		
		if (data[i] <= 0)
			break;
	}

	file << std::endl;

	file.close();
}

int main(int argc, char* argv[])
{
	MathSolver ms;
	ms.setDebug(false);
	ms.setLimit(100);
	bool saveResults = false;

	RNGesus rng;
	ms.setSeed(time(NULL));
	ms.setSeed(2);
	int sampleSize = 1;
	Genome result;
	Stopwatch watch;

	while (1)
	{
		watch = Stopwatch();

		std::cout << "samplesize: " << sampleSize << std::endl;

		std::cout << "*******\n1+1\n";
		watch.start();
		ms.setMode(MathSolver::ONEPLUSONE);
		testing(ms, sampleSize, result);
		if(saveResults)
			SaveQuality("1+1.csv", "1+1", ms.qualityCourse);

		std::cout << "*******\nMu+Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::PLUS);
		ms.setInheritance(MathSolver::NONE);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("Mu+Lambda.csv", "MU+LAMBDA", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::NONE);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("MuCommaLambda.csv", "MUCOMMALAMBDA", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda COMBINE\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::COMBINE);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("MuCommaLambdaCombine.csv", "MUCOMMALAMBDA with COMBINE", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda BLEND\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::BLEND);
		ms.setMu(10);
		ms.setLambda(100);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("MuCommaLambdaBlend.csv", "MUCOMMALAMBDA with BLEND", ms.qualityCourse);

		char input[2];
		std::cin.getline(input, 2);
		std::cin.clear();
		if (input[0] == 'q')
			break;
	}

	NQueens nq;
	nq.setSeed(time(NULL));
	while (1)
	{
		//NQueens
		Queenome result;
		//result.data = { 5,0,1,6,3,7,2,4 };
		int steps = nq.Solve(8, 100, 10000, &result);
		std::cout << result << std::endl;
		std::cout << nq.Fitness(result) << std::endl;
		Kingdom::printQueenome(result);
		
		char input[2];
		std::cin.getline(input, 2);
		std::cin.clear();
		if (input[0] == 'q')
			break;
	}
}
