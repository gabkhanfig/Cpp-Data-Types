# Custom Data Types C++

<h3>Currently added types:</h3>

- Dynamic array
- String
- Bitset

<h2>Dynamic Array</h2>

Array of any type, that changes size dynamically. A replacement to std::vector, that has a size of 16 bytes. Allows the use of custom allocator size increase functions. Also allows construction time capacity reservation. Error messages are also more helpful.

Array is able to do the following:

- Custom size increasing from user defined or default allocator.
- Construction time capacity reservation.
- Adding an element (growing the array if necessary).
- Adding many elements (doing a single reallocation if necessary).
- Adding many polymorphic elements (same as above but supports type casting).
- Appending another array.
- Check if contains an element.
- Finding the nth occurrence element.
- Finding the last occurrence of an element.
- Removing the nth occurrence of an element and shifting array.
- Removing all occurrences of an element and shifting array.
- Shrink an array so that it's capacity is equal to the amount of stored elements.
- Insert an element at a specific index, shifting the array.
- Remove an element at a specific index, shifting the array.

<h2>String</h2>

<h2>Bitset</h2>