#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include "types/array/Array.h"
#include "types/string/String.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#define print(message) std::cout << message << '\n'

class Base {
	int a;
};

class Child : public Base {
	int b;
};

int main() 
{
	const int iterations = 100000000;
	Benchmark* myString = Benchmark::StartBenchmark("my string");
	for (int i = 0; i < iterations; i++) {
		String small = "small";
		String large = "long string 123123123123123123123";
		String small2 = small + small;
		String midsize = "mediummedium";
		String large2 = large + large;
		String combo = small + large;
		String barely = midsize + midsize;
	}
	Benchmark::EndBenchmark(myString);
	Benchmark* stdString = Benchmark::StartBenchmark("std string");
	for (int i = 0; i < iterations; i++) {
		std::string small = "small";
		std::string large = "long string 123123123123123123123";
		std::string small2 = small + small;
		std::string midsize = "mediummedium";
		std::string large2 = large + large;
		std::string combo = small + large;
		std::string barely = midsize + midsize;
	}
	Benchmark::EndBenchmark(stdString);

	//String Small = "Hello World!";
	//String Long = "Super duper uber mega giga long string";
	//String LongDupe = Long;
	////String combo = String::ConcatenateStrings(Small, Long);
	//Small.AppendString(Long + LongDupe);
	//std::cout << Small << std::endl;

	//std::string small = "stdsmall ";
	//std::string small2 = small + small;
	//std::cout << small << std::endl;
	//std::cout << small2 << std::endl;

	//std::cout << String("ball ") + String("titties") << std::endl;
}