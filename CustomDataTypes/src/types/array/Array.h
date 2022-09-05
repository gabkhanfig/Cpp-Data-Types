#pragma once
#include <utility>

#define ARRAY_EXTERN extern

#ifdef _DEBUG == 1
#define ARRAY_DEBUG
#endif

#ifdef ARRAY_DEBUG

#endif

/* Function pointer of array name. */
typedef const char* (*array_name)();

/* Integer size type for array. */
typedef unsigned int ArrInt;

/* Function pointer of function that will output the new capacity value from the current one, when increasing size. */
typedef ArrInt (*array_capacity_increase)(ArrInt);

ARRAY_EXTERN ArrInt _ArrayCapacityIncrease(ArrInt currentCapacity);
ARRAY_EXTERN void _ArrayError(const char* errorMessage);

/*
Dynamic Array:
Array of dynamically changing size.

@param - T: Type
@param - capacityInc (optional. default ArrayDefaultCapacityIncrease): Pointer to a function that returns a new capacity from the current one for increasing.
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

	/* Default constructor */
	Array(ArrInt initialCapacity = 1)
	{
		data = new T[initialCapacity];
		capacity = initialCapacity;
		size = 0;
#ifdef ARRAY_NAME
		name = "dynamic_array";
#endif
	}

	/* Initializer list constructor */
	Array(const std::initializer_list<T>& il)
	{
		size = il.size();
		capacity = il.size();

		data = new T[capacity];
		ArrInt incrementor = 0;
		for (const T& i : il) {
			data[incrementor] = i;
			incrementor++;
		}
#ifdef ARRAY_NAME
		name = "dynamic_array";
#endif
	}

	/* Copy constructor */
	Array(const Array<T, capacityInc>& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];

		for (ArrInt i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
#ifdef ARRAY_NAME
		name = other.name;
#endif
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
	void Reallocate(ArrInt newCapacity)
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
	inline ArrInt Size() { return size; }

	/* Get size of array. */
	inline ArrInt Num() { return size; }

	/* Get the current capacity of the array. */
	inline ArrInt Capacity() { return capacity; }

	/* Get a reference to an element at a specific index. Will only check if correct index if ARRAY_CHECK_OUT_OF_BOUNDS macro is set.
	@param index: Array index to check.
	@returns Reference to the item in the array. */
	T& At(ArrInt index) 
	{
		if (index >= size) {
			ArrayError("index out of bounds from Array::At(). aborting");
		}
		return data[index];
	}

	/* Index of operator. 
	@param index: Array index to check.
	@returns Reference to the item in the array. */
	T& operator [] (ArrInt index) 
	{
		return At(index);
	}

	/* Increase array capacity to the supplied value IF the current capacity is less than the supplied.
	@param newCapacity: The new capacity this array will hold. */
	void Reserve(ArrInt newCapacity) 
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

	/* Insert N elements from an initializer list. Only performs one capacity increase. 
	@param il: Initializer list values. */
	void InsertElements(const std::initializer_list<T>& il) 
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
	void InsertElements(const T* start, ArrInt amount)
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
	void InsertElementsCast(const U* start, ArrInt amount) 
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
	void AppendArray(const Array<T, capacityInc>& other) 
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
	void operator += (const Array<T, capacityInc>& other)
	{
		AppendArray(other);
	}

	/* Check if the array contains a given element. Optionally pass out the index of the first found instance of that element.
	@param element: Comparison value. 
	@param indexOut (optional): Pointer to array integer type to insert the found element index. */
	bool Contains(const T& element, ArrInt* indexOut = nullptr) 
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
	T* Find(const T& element, ArrInt* indexOut = nullptr, ArrInt occurrence = 1) 
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
	T* FindLast(const T& element, ArrInt* indexOut = nullptr) 
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
	void Remove(const T& element, ArrInt occurrence = 1)
	{
		ArrInt occurrenceCount = 1;
		for (ArrInt i = 0; i < size; i++) {
			if (element == At(i)) {
				if (occurrenceCount == occurrence) {
					for (ArrInt j = (i); j < (size - 1); j++) {
						data[j] = std::move(data[j + 1]);
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
	ArrInt RemoveAll(const T& element) 
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
	void Shrink() 
	{
		Reallocate(size);
	}

	/* Insert an element in the array at a specified index, shifting all elements after it over by 1.
	@param element: Element to insert into the array.
	@param index: Index to insert at. */
	void InsertAt(const T& element, ArrInt index) 
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
	void RemoveAt(ArrInt index, T* outElement = nullptr) 
	{
		if (index >= size) {
			ArrayError("index out of bounds from Array::RemoveAt(). aborting");
		}

		if (outElement) {
			*outElement = At(index);
		}

		for (ArrInt i = index; i < size - 1; i++) {
			data[i] = std::move(data[i + 1]);
		}
		size--;
	}
};

#undef ARRAY_EXTERN
#undef ARRAY_DEBUG