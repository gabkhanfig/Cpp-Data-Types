#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include "types/array/Array.h"
#include "types/string/String.h"
#include "types/map/Map.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#define print(message) std::cout << message << '\n'

void destr() {
	for (int i = 0; i < 100000000; i++) {
		
	}
}

int main() 
{
	destr();
	//Array<Array<int>> arr(4);
	Map<int, int> map;
	print("a");
	std::cin.get();
}