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
	typedef String string;

	const int iterations = 100000000;
	Benchmark* myString = Benchmark::StartBenchmark("My String");
	for (int i = 0; i < iterations; i++) {
		string small = "small";
		string large = "long string 123123123123123123123";
		string small2 = small + small;
		string midsize = "mediummedium";
		string large2 = large + large;
		string combo = small + large;
		string barely = midsize + midsize;
	}
	Benchmark::EndBenchmark(myString);
}