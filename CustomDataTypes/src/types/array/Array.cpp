#include "Array.h"
#include <iostream>
#include <string>
#include <numeric>

void _ArrayError(const char* errorMessage) 
{
	std::cout << "[ARRAY ERROR]: " << errorMessage << '\n';
}

#define _pragmsg(s) _Pragma(#s)
#define PRAGMA_MESSAGE(msg) _pragmsg(message(#msg))

#define TEST_ASSERT(test) \
PRAGMA_MESSAGE([RUN UNIT TEST]:  test);	\
static_assert(test, "[Array Compile Unit Test]: " #test)

//#define RUN_UNIT_TESTS_AT_COMPILE
#ifdef RUN_UNIT_TESTS_AT_COMPILE
namespace ArrayCompileUnitTests
{
	/* Add a single element to an array and get back out that element. */
	constexpr bool ArraySingleAdd() 
	{
		auto arr = Array<int>();
		arr.Add(15124);
		auto tmp = arr[0];
		return tmp == 15124;
	}
	TEST_ASSERT(ArraySingleAdd());

	/* Reserve a specified capacity of the array and ensure that the array  */
	constexpr bool ArrayReserveCapacity() 
	{
		auto arr = Array<int>();
		arr.Reserve(123456);
		return arr.Capacity() == 123456;
	}
	TEST_ASSERT(ArrayReserveCapacity());

	/* Ensure dynamic allocator does indeed allocate enough. */
	constexpr bool ArrayCorrectBounds() 
	{
		auto arr = Array<int>();
		arr.Reserve(1000);
		arr.GetRawData()[999] = 7;
		return arr.GetRawData()[999] == 7;
	}
	TEST_ASSERT(ArrayCorrectBounds());

	/* Add multiple elements to an array, verifying it's size increases appropriately. */
	constexpr bool ArrayMultiAdd() 
	{
		auto arr = Array<int>();
		for (int i = 0; i < 20; i++) {
			arr.Add(i);
			if (arr[i] != i) return false;
		}
		return arr.Size() == 20;
	}
	TEST_ASSERT(ArrayMultiAdd());

	/* Insert multiple elements by initializer list. */
	constexpr bool ArrayInsertElementsInitializerList() 
	{
		auto arr = Array<int>();
		arr.InsertElements({ 1, 6, 4, 2, 6, 8, 2, 65, 7, 1, 2, 6 });
		return arr.Size() == 12;
	}
	TEST_ASSERT(ArrayInsertElementsInitializerList());

	/* Insert multiple elements by pointer and count. */
	constexpr bool ArrayInsertElementsPointer() 
	{
		auto arr = Array<int>();
		int* nums = new int[100];
		for (int i = 0; i < 100; i++) {
			nums[i] = i;
		}
		arr.InsertElements(nums, 100);
		delete[] nums;
		return arr.Size() == 100;
	}
	TEST_ASSERT(ArrayInsertElementsPointer());

	/* Append one array onto another. */
	constexpr bool ArrayAppendOther() 
	{
		Array<int> a = { 1, 2, 3, 4, 5, 6, 7, 8 };
		Array<int> b = { 1, 2, 4, 8, 16, 32, 128, 256 };
		a += b;
		return a.Size() == 16;
	}
	TEST_ASSERT(ArrayAppendOther());

	/* Does array contain a given element. */
	constexpr bool ArrayContains() 
	{
		Array<int> arr = { 90, 100, 236236, 28, 2 };
		return arr.Contains(236236);
	}
	TEST_ASSERT(ArrayContains());

	/* Find an item in the array along with it's index. */
	constexpr bool ArrayFind() 
	{
		Array<int> arr = { 90, 100, 236236, 28, 2 };
		ArrInt index;
		if (arr.Find(28, &index)) {
			return index == 3;
		}
		else {
			return false;
		}
	}
	TEST_ASSERT(ArrayFind());

	/* Find the nth occurrence of an item in the array. */
	constexpr bool ArrayFindOccurrence() 
	{
		Array<int> arr = { 200, 1, 1, 201, 1, 25, 52 };
		ArrInt index;
		if (arr.Find(1, &index, 3)) {
			return index == 4;
		}
		else {
			return false;
		}
	}
	TEST_ASSERT(ArrayFindOccurrence());

	/* Find the last occurrence of an item in the array. */
	constexpr bool ArrayFindLast() 
	{
		Array<int> arr = { 200, 1, 1, 201, 1, 25, 52 };
		ArrInt index;
		if (arr.FindLast(1, &index)) {
			return index;
		}
		else {
			return false;
		}
	}
	TEST_ASSERT(ArrayFindLast());

	/* Ensure array removes elements and keeps order upon removal of one. */
	constexpr bool ArrayRemove() 
	{
		Array<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		arr.Remove(5);
		return arr.Size() == 9;
	}
	TEST_ASSERT(ArrayRemove());

	/* Ensure array removes elements and keeps order upon removal of an occurrence of an element. */
	constexpr bool ArrayRemoveOccurrence() 
	{
		Array<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		arr.Remove(6, 2);
		return arr[15] != 6;
	}
	TEST_ASSERT(ArrayRemoveOccurrence());

	/* Remove all occurrences of an element of the array, maintaining order of all other elements. */
	constexpr bool ArrayRemoveAll() 
	{
		Array<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		arr.RemoveAll(4);
		return arr.Size() == 18;
	}
	TEST_ASSERT(ArrayRemoveAll());

	/* Shrink the array to fit the amount of data it contains. */
	constexpr bool ArrayShrink() 
	{
		Array<int> arr;
		arr.Reserve(1000);
		arr.Add(1);
		arr.Shrink();
		return arr.Capacity() == arr.Size();
	}
	TEST_ASSERT(ArrayShrink());

	/* Insert an element at a specific index, shifting the arrays elements if necessary. */
	constexpr bool ArrayInsertAt() 
	{
		Array<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8 };
		arr.InsertAt(9, 1);
		return arr[1] == 9;
	}
	TEST_ASSERT(ArrayInsertAt());

	/* Remove an element at a specific index, preserving order of the leftover elements. */
	constexpr bool ArrayRemoveAt() 
	{
		Array<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8 };
		int elem;
		arr.RemoveAt(1, &elem);
		return (arr.Size() == 7) && (elem == 2);
	}
	TEST_ASSERT(ArrayRemoveAt());

	/* Copy an array into another one. */
	constexpr bool ArrayCopy() 
	{
		Array<int> a;
		Array<int> b = { 5, 4, 2, 6, 2, 5 };
		a = b;
		return (a.Size() == b.Size()) && (a[2] == 2);
	}
	TEST_ASSERT(ArrayCopy());

	/* Concatenate two arrays into a new third array. */
	constexpr bool ArrayConcatenate() 
	{
		Array<int> a = { 1, 54, 25, 6, 2, 6, 5 };
		Array<int> b = { 5, 4, 2, 6, 2, 5 };
		Array<int> c = a + b;
		return (c.Size() == (a.Size() + b.Size())) && (c[0] == 1) && (c[a.Size()] == 5);
	}
	TEST_ASSERT(ArrayConcatenate());

	/* Try to get the index of a valid and invalid element. */
	constexpr bool ArrayTryGetIndex() 
	{
		Array<int> a = { 1, 54, 25, 6, 2, 6, 5 };
		Array<int> b = { 5, 4, 2, 6, 2, 5 };

		ArrInt aOutIndex;
		const bool aIndex = a.TryGetIndex(54, aOutIndex);

		ArrInt bOutIndex;
		const bool bIndex = b.TryGetIndex(1005401, bOutIndex);

		return aIndex && (aOutIndex == 1) && !bIndex;
	}
	TEST_ASSERT(ArrayTryGetIndex());

	/* Fill an array up to it's maximum allocated capacity with a specified element. */
	constexpr bool ArrayFillWith() 
	{
		Array<int> arr;
		arr.Reserve(10);
		arr.FillWith(55);
		for (int i = 0; i < 10; i++) {
			if (arr[i] != 55) return false;
		}
		return true;
	}
	TEST_ASSERT(ArrayFillWith());
}
#endif
