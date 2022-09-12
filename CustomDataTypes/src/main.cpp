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

	String str = "i want candy";
	String sub = str.Substring(4, 12);
	print(sub);

	//print(sub[6]);

	String splitter = "hello, there, what's, up, today";
	auto arr = splitter.Split(", ");
	for (auto s : arr) {
		print(s);
	}
}