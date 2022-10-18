#pragma once

#include <utility>
#include <type_traits>
#include <iostream>

#ifndef ARRAY_CHECK_OUT_OF_BOUNDS
#define ARRAY_CHECK_OUT_OF_BOUNDS true
#endif

/* Integer size type for array. */
typedef unsigned int ArrInt;

/* Function pointer of function that will output the new capacity value from the current one, when increasing size. */
typedef ArrInt (*array_capacity_increase)(ArrInt);

//extern ArrInt _ArrayCapacityIncrease(ArrInt currentCapacity);
extern void _ArrayError(const char* errorMessage);

constexpr ArrInt _ArrayCapacityIncrease(ArrInt currentCapacity)
{
	if (currentCapacity == 0) {
		return 2;
	}
	else if (currentCapacity < 5) {
		return (currentCapacity + 1) * 2;
	}
	else {
		return ArrInt(double(currentCapacity) * 1.618);
	}
}

/*
Array of dynamically changing size.
Optionally disable out of bounds checks for indexing 
by setting the ARRAY_CHECK_OUT_OF_BOUNDS macro to false by doing
-- #define ARRAY_CHECK_OUT_OF_BOUNDS false -- before including this array header

@param - T: Type the array contains
@param - capacityInc (optional. default _ArrayCapacityIncrease): Pointer to a function that returns a new capacity from the current one for increasing.
*/
template<
	typename T,
	array_capacity_increase capacityInc = _ArrayCapacityIncrease>
struct Array
{
private:

	/* Pointer to data block. */
	T* data;

	/* The amount of elements currently stored in the array. */
	ArrInt size;

	/* The amount of elements that this array can hold without reallocation. */
	ArrInt capacity;

public:

	/* Array custom iterator. */
	class iterator 
	{
	public:

		constexpr iterator(T* _data) : data(_data) {}

		iterator operator++() { ++data; return *this; }

		bool operator!=(const iterator& other) const { return data != other.data; }

		const T& operator*() const { return *data; }

	private:

		T* data;
	};

public:

	/* Default constructor */
	constexpr Array()
	{
		data = new T[1];
		capacity = 1;
		size = 0;
	}

	/* Initializer list constructor */
	constexpr Array(const std::initializer_list<T>& il)
	{
		size = il.size();
		capacity = il.size();

		data = new T[capacity];
		ArrInt incrementor = 0;
		for (const T& i : il) {
			data[incrementor] = i;
			incrementor++;
		}
	}

	/* Copy constructor */
	constexpr Array(const Array<T, capacityInc>& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];

		for (ArrInt i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	/* Destructor. Does not call destructors of data held within. */
	constexpr ~Array()
	{
		delete[] data;
	}
	
	constexpr iterator begin() const { return iterator(data); }
	constexpr iterator end() const { return iterator(data + size); }

private:

	/* Increase the capacity of the array from the provided capacity allocator. */
	constexpr void IncreaseCapacityFromAllocator() 
	{ 
		Reallocate(capacityInc(capacity));
	}

	/* Display an error message and abort the program.
	Uses a specified message along with the provided array name (for debugging purposes). */
	void ArrayError(const char* errorMessage)
	{
		_ArrayError(errorMessage);
		abort();
	}

	/* Reallocate the array to store a new capacity of elements.
	If the new capacity is less than the current array size, array size is decreased to new capacity.
	Be cautious of memory leaks due to array shrinking. */
	constexpr void Reallocate(ArrInt newCapacity)
	{
		T* newData = new T[newCapacity];

		if (newCapacity < size) {
			size = newCapacity;
		}

		for (ArrInt i = 0; i < size; i++) {
			newData[i] = std::move(data[i]);
		}

		delete[] data;
		data = newData;
		capacity = newCapacity;
	}

public:

	/* Get size of array. */
	constexpr inline ArrInt Size() const { return size; }

	/* Get the current capacity of the array. */
	constexpr inline ArrInt Capacity() const { return capacity; }

	/* Get a reference to an element at a specific index. Will only check if correct index if ARRAY_CHECK_OUT_OF_BOUNDS macro is set.
	@param index: Array index to check.
	@returns Reference to the item in the array. */
	constexpr T& At(ArrInt index)
	{
		#if ARRAY_CHECK_OUT_OF_BOUNDS == true
		if (index >= size) {
			ArrayError("index out of bounds from Array::At(). aborting");
		}
		#endif
		return data[index];
	}

	/* Index of operator. 
	@param index: Array index to check.
	@returns Reference to the item in the array. */
	constexpr T& operator [] (ArrInt index) 
	{
		return At(index);
	}

	/* Increase array capacity to the supplied value IF the current capacity is less than the supplied.
	@param newCapacity: The new capacity this array will hold. */
	constexpr void Reserve(ArrInt newCapacity) 
	{
		if (newCapacity > capacity) {
			Reallocate(newCapacity);
		}
	}

	/* Increase array capacity by a specified amount.
	@param extraCapacity: The amount to add to the array's current capacity. */
	void ReserveExtra(ArrInt extraCapacity) 
	{
		Reallocate(capacity + extraCapacity);
	}

	/* Add element to the end of the array by const ref. */
	constexpr void Add(const T& value) 
	{
		if (size == capacity) {
			IncreaseCapacityFromAllocator();
		}

		if (std::is_destructible<T>::value) {
			data[size].~T();
		}
		data[size] = value;
		size++;
	}

	/* Add element to the end of the array by r value reference. */
	constexpr void Add(T&& value) 
	{
		if (size == capacity) {
			IncreaseCapacityFromAllocator();
		}

		if (std::is_destructible<T>::value) {
			data[size].~T();
		}
		data[size] = value;
		size++;
	}

	/* Insert N elements from an initializer list. Only performs one capacity increase. 
	@param il: Initializer list values. */
	constexpr void InsertElements(const std::initializer_list<T>& il) 
	{
		const ArrInt ilSize = il.size();
		if (size + ilSize >= capacity) {
			Reallocate(size + ilSize);
		}

		for (const T& i : il) {
			data[size] = i;
			size++;
		}
	}

	/* Insert N elements from a specified start element. Only performs one capacity increase. 
	@param start: Pointer to an array of T value(s).
	@param amount: amount of T's to insert. */
	constexpr void InsertElements(const T* start, ArrInt amount)
	{
		if (size + amount >= capacity) {
			Reallocate(size + amount);
		}

		for (int i = 0; i < amount; i++) {
			data[size] = start[i];
			size++;
		}
	}

	/* Insert N elements to this array of a type that can be statically cast to the array T type. 
	@param start: Pointer to an array of U value(s).
	@param amount: amount of U's to insert. */
	template<typename U>
	constexpr void InsertElementsCast(const U* start, ArrInt amount) 
	{
		if (size + amount >= capacity) {
			Reallocate(size + amount);
		}

		for (ArrInt i = 0; i < amount; i++) {
			data[size] = static_cast<T>(start[i]);
			size++;
		}
	}

	/* Add the contents of another array to this one. */
	constexpr void AppendArray(const Array<T, capacityInc>& other) 
	{
		const ArrInt newSize = size + other.size; 
		if (newSize > capacity) {
			Reallocate(newSize);
		}

		for (ArrInt i = 0; i < other.size; i++) {
			data[size] = other.data[i];
			size++;
		}
	}

	/* Append another array to this one. */
	constexpr void operator += (const Array<T, capacityInc>& other)
	{
		AppendArray(other);
	}

	/* Check if the array contains a given element. Optionally pass out the index of the first found instance of that element.
	@param element: Comparison value. 
	@param indexOut (optional): Pointer to array integer type to insert the found element index. */
	constexpr bool Contains(const T& element, ArrInt* indexOut = nullptr) 
	{
		for (ArrInt i = 0; i < size; i++) {
			if (element == At(i)) {
				if (indexOut) {
					*indexOut = i;
				}
				return true;
			}
		}
		return false;
	}

	/* Iterate through the entire array, checking if the element matches. 
	Optional parameter occurence allows to specify to find the nth occurrence of the element (value of 1 = first). 
	@param element: Comparison value. 
	@param indexOut (optional): Pointer to array integer type to insert the found element index. 
	@param occurrence (optional): nth occurrence of the element.
	@returns Pointer to the found element. nullptr if not found. */
	constexpr T* Find(const T& element, ArrInt* indexOut = nullptr, ArrInt occurrence = 1) 
	{
		ArrInt occurrenceCount = 1;
		for (ArrInt i = 0; i < size; i++) {
			if (element == At(i)) {
				if (occurrenceCount == occurrence) {
					if (indexOut) {
						*indexOut = i;
					}
					return &At(i);
				}
				else {
					occurrenceCount++;
				}
			}
		}
		return nullptr;
	}

	/* Iterate through the entire array from the end to front, checking if the element matches.
	@param element: Comparison value. 
	@param indexOut (optional): Pointer to array integer type to insert the found element index.
	@returns Pointer to the found element. nullptr if not found. */
	constexpr T* FindLast(const T& element, ArrInt* indexOut = nullptr) 
	{
		bool reachedEnd = false;
		ArrInt i = size - 1;
		while (!reachedEnd) {
			if (element == At(i)) {
				if (indexOut) {
					*indexOut = i;
				}
				return &At(i);
			}
			if (i == 0) {
				reachedEnd = true;
			}
			i--;
		}
		return nullptr;
	}

	/* Remove the nth instance of an element from the array. 
	Doesn't shrink array, but does decrement size if item is found. 
	@param element: Comparison value. 
	@param occurrence (optional): nth occurrence of the element. */
	constexpr void Remove(const T& element, ArrInt occurrence = 1, bool destructIfCan = true)
	{
		ArrInt occurrenceCount = 1;
		for (ArrInt i = 0; i < size; i++) {
			if (element == At(i)) {
				if (occurrenceCount == occurrence) {

					if (std::is_destructible<T>::value) {
						std::cout << "manually destroying" << std::endl;
						if (i == size - 1) {
							At(i).~T();
						}
					}

					for (ArrInt j = (i); j < (size - 1); j++) {
						data[j] = std::move(data[j + 1]);
						//memmove(&data[j + 1], &data[size - 1], sizeof(T) * ((size + 1) - (j + 1)));
					}
					size--;
					return;
				}
				else {
					occurrenceCount++;
				}			
			}
		}
	}

	/* Remove all occurrences of an element in the array. 
	Doesn't shrink array, but does decrement size if item is found.
	@param element: Comparison value. 
	@returns Amount of elements removed from the array. */
	constexpr ArrInt RemoveAll(const T& element) 
	{
		ArrInt amount = 0;
		ArrInt i = 0;

		T* newData = new T[capacity];
		
		for (int ind = 0; ind < size; ind++) {
			if (data[ind] != element) {
				newData[i] = std::move(data[ind]);
				i++;
			}
			else {
				amount++;
			}
		}
		
		delete[] data;
		data = newData;
		size = i;

		return amount;
	}

	/* Shrinks the array to the smallest possible size to store all elements. */
	constexpr void Shrink() 
	{
		Reallocate(size);
	}

	/* Insert an element in the array at a specified index, shifting all elements after it over by 1.
	@param element: Element to insert into the array.
	@param index: Index to insert at. */
	constexpr void InsertAt(const T& element, ArrInt index) 
	{
		if (index >= size) {
			ArrayError("index out of bounds from Array::InsertAt(). aborting");
		}

		if (size + 1 >= capacity) {
			IncreaseCapacityFromAllocator();
		}

		size++;

		for (ArrInt i = size - 1; i > index; i--) {
			data[i] = std::move(data[i - 1]);
		}

		data[index] = element;
	}

	/* Remove an element at a specified index, shifting all elements after it over by 1.
	@param index: Index to remove.
	@param outElement (optional): Pointer to element type to copy the removed element to. */
	constexpr void RemoveAt(ArrInt index, T* outElement = nullptr) 
	{
		if (index >= size) {
			ArrayError("index out of bounds from Array::RemoveAt(). aborting");
		}

		if (outElement) {
			*outElement = At(index);
		}
		else {
			if (std::is_destructible<T>::value) {
				if (index == size - 1) {
					At(index).~T();
				}
			}
			// destructed automatically
			//At(index).~T();
		}

		for (ArrInt i = index; i < size - 1; i++) {
			data[i] = std::move(data[i + 1]);
		}
		size--;
	}

	/* Set this array equal to another. */
	constexpr void operator = (const Array<T, capacityInc> other)
	{
		if (data)
			delete[] data;

		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];

		for (ArrInt i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	/* Concatenate two arrays into a new one. */
	constexpr friend Array<T> operator + (const Array<T, capacityInc>& left, const Array<T, capacityInc>& right)
	{
		Array<T> arr;
		arr.Reserve(left.Size() + right.Size());
		arr.InsertElements(left.data, left.Size());
		arr.InsertElements(right.data, right.Size());
		return arr;
	}

	/* Try to get the index of an element in the array.
	@param element: The element being searched for.
	@param outIndex: Out param for the found index.
	@return If the index was successfully found.*/
	constexpr bool TryGetIndex(const T& element, ArrInt& outIndex) 
	{
		for (ArrInt i = 0; i < Size(); i++) {
			if (At(i) == element) {
				outIndex = i;
				return true;
			}
		}
		return false;
	}

	/* Fill the array starting at the current size, until the max capacity, with a specific element. */
	constexpr void FillWith(const T& element) 
	{
		for (ArrInt i = Size(); i < Capacity(); i++) {
			Add(element);
		}
	}

	/* DANGEROUS!!!!! */
	constexpr T* GetRawData() 
	{
		return data;
	}
};