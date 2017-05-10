#include <iostream>
#include <ctime>
#include <fstream>

#include "MathSolver.h"
#include "NQueens.h"
#include "Stopwatch.h"

#define DEBUG(x) std::cout << #x << ": " << x << std::endl;

std::ostream& operator<<(std::ostream& os, const Genome& g) {
	return os	/*<< "x: "*/ << g.x << ","
		/*<< "y: "*/ << g.y << ","
		/*<< "a: "*/ << g.a << ","
		/*<< "b:" */ << g.b;
}

std::ostream& operator<<(std::ostream& os, const Queenome& q) {
	for (int i = 0; i < q.data.size(); ++i)
	{
		os << q.data[i];
		if (i < q.data.size() - 1)
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
	bool debug = false,
		saveResults = false;
	int sampleSize = 1,
		populationSize = 1000,
		mu = 10,
		lambda = 100,
		iterations = 10000,
		startLimit = 100,
		seed = time(NULL),
		boardSize = 8;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0)
		{
			debug = true;
			std::cout << "DEBUG MODE ACTIVATED" << std::endl;
		}
		else if (strcmp(argv[i], "--samples") == 0)
		{
			sampleSize = atoi(argv[++i]);
			if (debug)
				DEBUG(sampleSize);
		}
		else if (strcmp(argv[i], "--population") == 0)
		{
			populationSize = atoi(argv[++i]);
			if (debug)
				DEBUG(populationSize);
		}
		else if (strcmp(argv[i], "--mulambda") == 0)
		{
			mu = atoi(argv[++i]);
			lambda = atoi(argv[++i]);
			if (debug)
			{
				DEBUG(mu);
				DEBUG(lambda);
			}
		}
		else if (strcmp(argv[i], "--limit") == 0)
		{
			startLimit = atoi(argv[++i]);
			if (debug)
				DEBUG(startLimit);
		}
		else if (strcmp(argv[i], "--seed") == 0)
		{
			seed = atoi(argv[++i]);
			if (debug)
				DEBUG(seed);
		}
		else if (strcmp(argv[i], "--board") == 0)
		{
			boardSize = atoi(argv[++i]);
			if (debug)
				DEBUG(boardSize);
		}
		if (strcmp(argv[i], "--save") == 0)
		{
			saveResults = true;
			DEBUG(saveResults);
		}
	}

	MathSolver ms;
	ms.setDebug(debug);
	ms.setLimit(startLimit);

	RNGesus rng;
	ms.setSeed(seed);
	Genome result;

	while (1)
	{
		Stopwatch watch;

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
		ms.setMu(mu);
		ms.setLambda(lambda);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("Mu+Lambda.csv", "MU+LAMBDA", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::NONE);
		ms.setMu(mu);
		ms.setLambda(lambda);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("MuCommaLambda.csv", "MUCOMMALAMBDA", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda COMBINE\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::COMBINE);
		ms.setMu(mu);
		ms.setLambda(lambda);
		testing(ms, sampleSize, result);
		if (saveResults)
			SaveQuality("MuCommaLambdaCombine.csv", "MUCOMMALAMBDA with COMBINE", ms.qualityCourse);

		std::cout << "*******\nMu,Lambda BLEND\n";
		ms.setMode(MathSolver::MULAMBDA);
		ms.setExactly(MathSolver::COMMA);
		ms.setInheritance(MathSolver::BLEND);
		ms.setMu(mu);
		ms.setLambda(lambda);
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
	nq.setDebug(debug);
	nq.setSeed(seed);
	while (true)
	{
		Stopwatch watch;
		//NQueens
		Queenome result;
		//result.data = { 5,0,1,6,3,7,2,4 };
		watch.start();
		int steps = nq.Solve(boardSize, populationSize, iterations, &result);
		watch.stop();
		std::cout << result << std::endl;
		std::cout << nq.Fitness(result) << std::endl;
		Kingdom::printQueenome(result);
		
		std::cout << "time: " << watch.getDurationString(0) << std::endl;

		char input[2];
		std::cin.getline(input, 2);
		std::cin.clear();
		if (input[0] == 'q')
			break;
	}
}
