#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include "types/array/Array.h"
#include "types/string/String.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#define print(message) std::cout << message << '\n'

void test_destruction() {
	Array<Array<int>> a;
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
	a.Add(Array<int>({ 1 }));
}

int main() 
{
	test_destruction();
}