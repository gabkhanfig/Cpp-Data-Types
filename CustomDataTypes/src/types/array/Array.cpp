#include "Array.h"

unsigned int ArrayDefaultCapacityIncrease(unsigned int currentCapacity) 
{
	return currentCapacity * 2;
}

const char* ArrayDefaultName() 
{
	return "unnamed_array";
}

void _ArrayError(const char* arrayName, const char* errorMessage) 
{
	std::cout << "[ARRAY ERROR]: \"" << arrayName << "\" " << errorMessage << '\n';
}