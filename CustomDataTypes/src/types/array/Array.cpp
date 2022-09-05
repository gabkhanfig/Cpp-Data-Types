#include "Array.h"
#include <iostream>
#include <string>

ArrInt _ArrayCapacityIncrease(ArrInt currentCapacity)
{
	if (currentCapacity == 0) {
		return 2;
	}
	else if (currentCapacity < 5) {
		return (currentCapacity + 1) * 2;
	}
	else {
		return ArrInt(float(currentCapacity) * 1.618f);
	}
}

void _ArrayError(const char* errorMessage) 
{
	std::cout << "[ARRAY ERROR]: " << errorMessage << '\n';
}