#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include "types/array/Array.h"
#include <bitset>
#include <iostream>
#include <vector>

#define print(message) std::cout << message << '\n'

const char* testArrName() { return "butt"; }

class Base {
	int a;
};

class Child : public Base {
	int b;
};

int main() 
{
	Array<int, ArrayDefaultCapacityIncrease, testArrName> arr;
	arr.Add(7);
	arr.Add(1);
	arr.Add(3);
	arr.Add(4);
	print(arr[0]);
	print(arr.GetName());

	auto arr2 = arr;
	print(arr2[0]);

	int* newInt = new int;
	*newInt = 10;

	print(arr.Size());

	arr.InsertElements({ 1, 5, 2, 4, 6, 8, 2, 1, 5, 7, 6, 4, 5 });
	print(arr.Size());

	arr += arr2;
	//print(arr.Size());

	Array<Base*> arrBase;
	arrBase.Add(new Base());
	Array<Child*> arrChild;
	arrChild.Add(new Child());
	arrChild.Add(new Child());
	arrChild.Add(new Child());
	Array<int> arrInt = { 1, 2, 3, 4 };
	//arrBase.Add(arrChild[0]);
	arrBase.InsertElementsCast(&arrChild[0], 1);

	
}