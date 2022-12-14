# Custom Data Types C++

<h3>Currently added types:</h3>

- Dynamic array
- String
- Bitset

**Unit testing** is used.

<h2>Dynamic Array</h2>

Array of any type, that changes size dynamically. A replacement to std::vector, that has a size of 16 bytes. Error messages are intended to be more helpful. **Unit testing at compile time**

Array is able to do the following:

- Custom size increasing from user defined or default allocator.
- Construction time capacity reservation.
- Iterator for range based for loop.
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
- Constexpr functionality.

<h2>String</h2>

String of byte sized chars. A replacement to std::string that supports [**Small String Optimization**](https://blogs.msmvps.com/gdicanio/2016/11/17/the-small-string-optimization/). This implementation differs by allowing small strings of up to a length of **32 characters** including the null terminator. Standard SSO implementations do not support this. The string (excluding heap string data) has a size of 32 bytes.

10 million iterations in O2. (Ryzen 7 1700x @ 3.85GHz, 16GB 3200MHz RAM)

```
typedef String string; // replace with std::string for that test

Benchmark* string_benchmark = Benchmark::StartBenchmark("My String");
for (int i = 0; i < iterations; i++) {
	string small = "small";
	string large = "long string 123123123123123123123";
	string small2 = small + small;
	string midsize = "mediummedium";
	string large2 = large + large;
	string combo = small + large;
	string barely = midsize + midsize;
}
Benchmark::EndBenchmark(string_benchmark);
```

> This String:        22689ms
>
> Msvc std::string:   27045ms


String is able to do the following:

- Small String Optimization. Any string of size less 33 characters including the null terminator will be allocated within the string data itself, not in the data pointer.
- Support long strings (33+ chars in the heap).
- Outperform msvc std::string.
- 32 byte size is a power of 2, thus (hopefully) fitting in cache lines cleanly.
- Append another string.
- Concatenate two strings into a new one.
- Printing with std::cout support.

<h2>Bitset</h2>

Bitset of variable specified bitsize. Occupies only as much space as is necessary.

> request bit amount <= 8: 1 byte (unsigned char)
>
> request bit amount <= 16: 2 bytes (unsigned short)
>
> request bit amount <= 32: 4 bytes (unsigned int)
>
> request bit amount <= 64: 8 bytes	(unsigned long long)
>
> request bit amount > 64: 16+ bytes (unsigned long long array)

Bitset is able to do the following:

- Small memory footprint for small bitsets (msvc std::bitset has a minimum size of 4 bytes).
- Construct empty bitset with 0s.
- Construct with pre-set flags.
- Construct with pre-set flags array.
- Index (supports index values greater than 63).
- Set the boolean flag state of a specific index (same index rules as above).
- Equivalency checks.
