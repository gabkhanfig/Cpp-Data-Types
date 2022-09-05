#include "Benchmark/Benchmark.h"

#include "Types/bitset/bitset.h"
#include "types/array/Array.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <array>

#define print(message) std::cout << message << '\n'

class Base {
	int a;
};

class Child : public Base {
	int b;
};

int main() 
{
	Array<int> a = { 3, 1, 4, 1, 1, 1, 5, 6, 4 };
	for (int i = 0; i < a.Size(); i++) {
		std::cout << a[i] << " ";
	}
	std::cout << '\n';
	std::cout << a.Size() << " " << a.Capacity() << '\n';

	a.InsertAt(8, 5);

	for (int i = 0; i < a.Size(); i++) {
		std::cout << a[i] << " ";
	}
	std::cout << '\n';
	std::cout << a.Size() << " " << a.Capacity() << '\n';
	
	int out;
	a.RemoveAt(5, &out);
	for (int i = 0; i < a.Size(); i++) {
		std::cout << a[i] << " ";
	}
	std::cout << '\n';
	std::cout << a.Size() << " " << a.Capacity() << '\n';
	std::cout << out;

}