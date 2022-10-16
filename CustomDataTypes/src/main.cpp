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

int main() {

	Array<int> a;
	print(a.Size());
}