#pragma once
#include <utility>
#include <string>
#include <iostream>

#define ARRAY_EXTERN extern

/* Function pointer of array name. */
typedef const char* (*array_name)();

/* Function pointer of function that will output the new capacity value from the current one, when increasing size. */
typedef unsigned int (*array_capacity_increase)(unsigned int);

ARRAY_EXTERN unsigned int ArrayDefaultCapacityIncrease(unsigned int currentCapacity);
ARRAY_EXTERN const char* ArrayDefaultName();
ARRAY_EXTERN void _ArrayError(const char* arrayName, const char* errorMessage);

/*
Dynamic Array:
Array of dynamically changing size.

@param - T: Type
@param - capacityAllocator (optional. default ArrayDefaultCapacityIncrease): Pointer to a function that returns a new capacity from the current one for increasing.
@param - str (optional. default ArrayDefaultName): Pointer to a function that returns a c-string representing the name of this array.
Primarily for debug use.
*/
template<
	typename T,
	array_capacity_increase capacityAllocator = ArrayDefaultCapacityIncrease,
	array_name str = ArrayDefaultName>
struct Array
{
private:

	/* Pointer to data block. */
	T* data;

	/* The amount of elements currently stored in the array. */
	unsigned int size;

	/* The amount of elements that this array can hold without reallocation. */
	unsigned int capacity;

public:

	/* Default constructor */
	Array(unsigned int initialCapacity = 1)
	{
		data = new T[initialCapacity];
		capacity = initialCapacity;
		size = 0;
	}

	/* Initializer list constructor */
	Array(const std::initializer_list<T>& il)
	{
		size = il.size();
		capacity = il.size();

		data = new T[capacity];
		unsigned int incrementor = 0;
		for (const T& i : il) {
			data[incrementor] = i;
			incrementor++;
		}
	}

	/* Copy constructor */
	Array(const Array<T, capacityAllocator, str>& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];

		for (int i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	/* Destructor. Does not call destructors of data held within. */
	~Array()
	{
		delete[] data;
	}

private:

	/* Increase the capacity of the array from the provided capacity allocator. */
	constexpr void IncreaseCapacityFromAllocator() 
	{ 
		Reallocate(capacityAllocator(capacity)); 
	}

	/* Display an error message and abort the program.
	Uses a specified message along with the provided array name (for debugging purposes). */
	void ArrayError(const char* errorMessage)
	{
		_ArrayError(GetName(), errorMessage);
		abort();
	}

	/* Reallocate the array to store a new capacity of elements.
	If the new capacity is less than the current array size, array size is decreased to new capacity.
	Be cautious of memory leaks due to array shrinking. */
	void Reallocate(unsigned int newCapacity)
	{
		T* newData = new T[newCapacity];

		if (newCapacity < size) {
			size = newCapacity;
		}

		for (unsigned int i = 0; i < size; i++) {
			newData[i] = std::move(data[i]);
		}

		delete[] data;
		data = newData;
		capacity = newCapacity;
	}

public:

	/* Get size of array. */
	inline unsigned int Size() { return size; }

	/* Get size of array. */
	inline unsigned int Num() { return size; }

	/* Get the current capacity of the array. */
	inline unsigned int Capacity() { return capacity; }
	
	/* Get the c-string name of this array. If not set in template by function pointer, will be "unnamed_array". */
	constexpr const char* GetName() { return str(); }

	/* Index of operator. */
	T& operator [] (unsigned int index) {
		if (index >= size) {
			ArrayError("index out of bounds. aborting");
		}
		return data[index];
	}

	/* Add element to the end of the array by const ref. */
	void Add(const T& value) 
	{
		if (size == capacity) {
			IncreaseCapacityFromAllocator();
		}

		data[size] = value;
		size++;
	}

	/* Add element to the end of the array by r value reference. */
	void Add(T&& value) 
	{
		if (size == capacity) {
			IncreaseCapacityFromAllocator();
		}

		data[size] = value;
		size++;
	}

	/* Insert N elements from an initializer list. Only performs one capacity increase. */
	void InsertElements(const std::initializer_list<T>& il) 
	{
		const unsigned int ilSize = il.size();
		if (size + ilSize >= capacity) {
			Reallocate(size + ilSize);
		}

		for (const T& i : il) {
			data[size] = i;
			size++;
		}
	}

	/* Insert N elements from a specified start element. Only performs one capacity increase. */
	void InsertElements(const T* start, unsigned int amount)
	{
		if (size + amount >= capacity) {
			Reallocate(size + amount);
		}

		for (int i = 0; i < amount; i++) {
			data[size] = start[i];
			size++;
		}
	}

	/* Insert N elements from this one with static cast. */
	template<typename U>
	void InsertElementsCast(U* start, unsigned int amount) 
	{
		if (size + amount >= capacity) {
			Reallocate(size + amount);
		}

		for (int i = 0; i < amount; i++) {
			data[size] = static_cast<T>(start[i]);
			size++;
		}
	}

	/* Add the contents of another array to this one. */
	void AppendArray(const Array<T, capacityAllocator, str>& other) 
	{
		const unsigned int newSize = size + other.size; 
		if (newSize > capacity) {
			Reallocate(newSize);
		}

		for (int i = 0; i < other.size; i++) {
			data[size] = other.data[i];
			size++;
		}
	}

	/* Append another array to this one. */
	void operator += (const Array<T, capacityAllocator, str>& other)
	{
		AppendArray(other);
	}

};

#undef ARRAY_EXTERN