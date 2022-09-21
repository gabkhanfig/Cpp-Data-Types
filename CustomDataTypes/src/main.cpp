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

#define print(message) std::cout << message << '\n'

int main() 
{
	SString a = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	SString b = "bbuhaaisudhgoaiusyd;alsup";
	a.Append(b);
	print(a);
}