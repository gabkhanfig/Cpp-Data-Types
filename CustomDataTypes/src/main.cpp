#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"

//#define ARRAY_CHECK_OUT_OF_BOUNDS false
#include "types/array/Array.h"

#include "types/string/String.h"
#include "types/string/SString.h"

#include "types/map/Map.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#include <immintrin.h>
#include "types/array/StaticArray.h"
#include <vector>

#define TEST

#define print(message) std::cout << message << '\n'

constexpr int* MakeVals() {
	const int size = 1;
	int* arr = new int[size];
	for (int i = 0; i < size; i++) {
		arr[i] = i;
	}
	return arr;
}

std::vector<int> MakeValsNonConstexpr() {
	const int size = 100000000;
	std::vector<int> someVals;
	someVals.reserve(size);
	for (int i = 0; i < size; i++) {
		someVals.push_back(i);
	}
	return someVals;
}

#ifdef TEST
Benchmark* bench = Benchmark::StartBenchmark("make vals");
constexpr const int* globalVals = MakeVals();
#endif

int main() {
#ifndef TEST
	Benchmark* bench = Benchmark::StartBenchmark("make vals");
	const std::vector<int> globalVals = MakeValsNonConstexpr();
#endif
	static_assert(globalVals);

	bench->EndBenchmark();
}