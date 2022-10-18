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

	const int count = 100000000;
	for (int i = 0; i < count; i++) {
		Array<Test> arr;
		Test t = Test();
		////Test t1 = Test();
		arr.Add(Test());
		////arr.Remove(t);
		//

		//std::vector<Test> vec;
		//vec.push_back(t);
		print(i);
	}
	
}