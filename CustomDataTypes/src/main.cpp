#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include <bitset>
#include <iostream>


int main() 
{
	bitset<5> a = 1;
	bitset<5> b = a;
	a = bitset<5>(1);
	a = 3;
	std::cout << a[0] << std::endl;
	std::cout << b[0] << std::endl;
	if (a == 3) {
		std::cout << "equal" << std::endl;
	}
	else {
		std::cout << "not equal" << std::endl;
	}

	sizeof(bitset<1>); // 1 byte
	sizeof(std::bitset<1>); // 4 bytes
}