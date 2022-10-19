#pragma once

//#include <utility>
#include <iostream>
#include <assert.h>
#include <utility>


#ifndef ARRAY_CHECK_OUT_OF_BOUNDS
#define ARRAY_CHECK_OUT_OF_BOUNDS true
#endif

template<typename T, size_t size>
struct sarray 
{
private:

	T data[size];

public:

	/* Get the total amount of elements this array can hold */
	constexpr size_t Size() const { return size; }

	sarray() {

	}

	template<typename U, size_t N>
	sarray(const U(&elements)[N]) {
		static_assert(N <= size, "Amount of array elements for construction exceeds static array capacity");
		for (int i = 0; i < N; i++) {
			data[i] = static_cast<T>(elements[i]);
		}
	}
	
	template<typename U>
	sarray(U* elements, size_t count) {
		count = count <= size ? count : size;
		for (int i = 0; i < count; i++) {
			data[i] = static_cast<T>(elements[i]);
		}
	}

	sarray(const sarray<T, size>& other) {
		for (int i = 0; i < size; i++) {
			data[i] = other[i];
		}
	}

	sarray(const std::initializer_list<T>& il) {
		size_t count = il.size() <= size ? il.size() : size;
		size_t i = 0;
		for (const auto& elem : il) {
			if (i == count) return;

			data[i] = std::move(elem);
			i++;
		}
	}

	/* Direct copy. */
	void operator = (sarray<T, size>& other) {
		for (int i = 0; i < size; i++) {
			data[i] = other[i];
		}
	}
	
	/*  */
	template <typename U, size_t N>
	void operator = (const U(&list)[N]) {
		static_assert(N <= size, "sarray operator '=' with list has more elements than array's capacity");
		for (int i = 0; i < N; i++) {
			data[i] = list[i];
		}
	}

	T& At(size_t index) {
#if ARRAY_CHECK_OUT_OF_BOUNDS == true
		if (index >= size) {
			std::cerr << "Attempting to get an element at an index that exceeds sarray size...\n Index: " << index << "\nStatic Array Size: " << size << '\n';
			assert(false);
		}
#endif
		return data[index];
	}

	T& operator [] (size_t index) {
		return At(index);
	}

	/* Copy elements into this array starting at the first element. */
	template<typename U>
	void CopyInto(U* elements, size_t count, size_t offset = 0) {
#if ARRAY_CHECK_OUT_OF_BOUNDS == true
		if (count + offset > size) {
			std::cerr << "Copying into array with a pointer to elements that exceeds this array's capacity...\n Elements Count: " << count << "\nStatic Array Size: " << size << "\nOffset: " << offset << '\n';
			assert(false);
		}
#endif
		for (int i = 0; i < count; i++) {
			data[i + offset] = static_cast<T>(elements[i]);
		}
	}

	void SetAllElements(const T& value) {
		for (int i = 0; i < size; i++) {
			data[i] = value;
		}
	}

	
};


