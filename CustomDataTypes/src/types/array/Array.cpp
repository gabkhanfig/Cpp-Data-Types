#include "Array.h"
#include <iostream>
#include <string>
#include <numeric>

void _ArrayError(const char* errorMessage) 
{
	std::cout << "[ARRAY ERROR]: " << errorMessage << '\n';
}

#define TEST_ASSERT(condition) static_assert(condition, "[Array Compile Unit Test]: " #condition)

namespace ArrayCompileUnitTests
{
	/* Add a single element to an array and get back out that element. */
	constexpr int ArraySingleAdd() {
		auto arr = Array<int>();
		arr.Add(15124);
		auto tmp = arr[0];
		return tmp;
	}
	TEST_ASSERT(ArraySingleAdd() == 15124);

	/* Reserve a specified capacity of the array and ensure that the array  */
	constexpr ArrInt ArrayReserveCapacity() {
		auto arr = Array<int>();
		arr.Reserve(123456);
		return arr.Capacity();
	}
	TEST_ASSERT(ArrayReserveCapacity() == 123456);

	constexpr int ArrayCorrectBounds() {
		auto arr = Array<int>();
		arr.Reserve(1000);
		arr.GetRawData()[999] = 7;
		return arr.GetRawData()[999];
	}
	TEST_ASSERT(ArrayCorrectBounds() == 7);

	constexpr ArrInt ArrayMultiAdd() {
		auto arr = Array<int>();
		for (int i = 0; i < 20; i++) {
			arr.Add(i);
		}
		return arr.Size();
	}
	TEST_ASSERT(ArrayMultiAdd() == 20);

	constexpr ArrInt ArrayInsertElementsInitializerList() {
		auto arr = Array<int>();
		arr.InsertElements({ 1, 6, 4, 2, 6, 8, 2, 65, 7, 1, 2, 6 });
		return arr.Size();
	}
	TEST_ASSERT(ArrayInsertElementsInitializerList() == 12);

	constexpr ArrInt ArrayInsertElementsPointer() {
		auto arr = Array<int>();
		int* nums = new int[100];
		for (int i = 0; i < 100; i++) {
			nums[i] = i;
		}
		arr.InsertElements(nums, 100);
		delete[] nums;
		return arr.Size();
	}
	TEST_ASSERT(ArrayInsertElementsPointer() == 100);
}
