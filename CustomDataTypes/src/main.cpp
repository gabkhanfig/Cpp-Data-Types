#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"

//#define ARRAY_CHECK_OUT_OF_BOUNDS false
#include "types/array/DynamicArray.h"

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

#define print(message) std::cout << message << '\n'

struct Test {
	int* a;

	Test() {
		a = new int;
	}

	~Test() {
		//print("Test destructor");
		delete a;
	}

	bool operator == (const Test& other) {
		return a == other.a;
	}

	void operator = (const Test& other) {
		*a = *other.a;
	}
};

int main() {

	sarray<int, 6> arr = { 1, 2, 5, 1, 6, 7, 12, 54 };

}