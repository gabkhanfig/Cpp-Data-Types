#include "Benchmark.h"
#include <iostream>

Benchmark* Benchmark::StartBenchmark(const std::string& benchmarkName)
{
	Benchmark* benchmark = new Benchmark();
	benchmark->name = benchmarkName;
	std::cout << "[Benchmark]: Starting benchmark \"" + benchmarkName + "\"" << '\n';
	benchmark->start = std::chrono::high_resolution_clock::now();
	return benchmark;
}

double Benchmark::EndBenchmark()
{
	end = std::chrono::high_resolution_clock::now();
	double benchmarkEnd = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "[Benchmark]: Benchmark \"" + name + "\" executed in " + std::to_string(benchmarkEnd) + " ms." << '\n';
	delete this;
	return benchmarkEnd;
}
