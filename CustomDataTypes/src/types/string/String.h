#pragma once

#include <iostream>

typedef unsigned char uint8;
typedef unsigned long long uint64;

constexpr uint64 MAX_SMALL_STRING_LENGTH = 31;

/*
Dynamically changing string, with small string optimization for strings of length 31 (excluding null terminator).
Occupies 32 bytes, same as msvc std::string, conviniently is a power of 2 to (hopefully) place multiple strings cleanly in cache lines.
Supremely outperforms msvc std::string in small string instantiation and concatenation,
and fairly outperforms std::string in long string instantiation and concatenation.
*/
struct String 
{
private:

	/**/
	union 
	{
		struct 
		{
			char* data;
			uint64 length;
			uint64 capacity;
			int _padding0;
			short _padding1;
			uint8 _padding2;
			uint8 flags;
		};
		struct 
		{
			char chars[MAX_SMALL_STRING_LENGTH + 1];
		};
	};

	/**/
	inline void SetIsSmallString() 
	{
		flags = flags & 223;
	}

	/**/
	inline void SetIsLongString() 
	{
		flags = flags | 32;
	}

	/**/
	inline uint64 SmallStringLength() const
	{
		return 31 - (flags & 31);
	}

	/**/
	inline uint64 LongStringLength() const
	{
		return length;
	}

	/**/
	inline void SetLengthSmall(uint8 SmallLength)
	{
		// Only the top 2 bits, not the "is long string" bit. This is an edge case ensuring a long string wont have it's flag size set.
		flags = (31 - SmallLength) + (flags & 192);
	}

	/**/
	inline void SetLengthLong(uint64 LongLength) 
	{ 
		length = LongLength;
	}

	/**/
	inline void SetLength(uint64 NewLength) 
	{
		if (NewLength <= MAX_SMALL_STRING_LENGTH) {
			SetLengthSmall(NewLength);
		}
		else {
			SetIsLongString();
			SetLengthLong(NewLength);
		}
	}

	/* Increases the capacity of the strings data by 1.5x whatever the inputted minimum is.
	Also forces this string to be considered a long string, regardless of the actual size of the data. */
	void IncreaseLongStringCapacity(uint64 MinCapacity);

public:

	/* Default constructor. Sets string to "". */
	String() 
	{
		chars[0] = '\0';
		SetLengthSmall(0); // strlen(Small.Chars);

		//std::cout << size << " size of empty string" << std::endl;
	}

	/* Construct with const char*. If inputted string has a length of 31 or less, this string will be a small string.
	Otherwise, this string will be a long string. */
	String(const char* _String) 
	{
		//std::cout << "const char* constructor" << '\n';
		SetLength(strlen(_String));
		if (IsSmallString()) {
			memcpy(chars, _String, SmallStringLength() + 1);
		}
		else {
			capacity = LongStringLength() + 1;
			data = new char[capacity];
			memcpy(data, _String, LongStringLength() + 1);
		}
	}

	/* Copy constructor. Currently duplicates the string data of the other string. */
	String(const String& Other) 
	{
		//std::cout << "copy constructor" << '\n';
		SetLength(Other.Length());
		if (IsSmallString()) {
			memcpy(chars, Other.chars, SmallStringLength() + 1);
			//Small = Other.Small;
		}
		else {
			data = new char[LongStringLength() + 1];
			memcpy(data, Other.data, LongStringLength() + 1);
			//strcpy_s(Long.Data, LongStringLength() + 1, Other.Long.Data);
			capacity = Other.capacity;
		}
	}

	/* Destructor. If is NOT small string, deletes the char data. */
	~String() 
	{
		if (!IsSmallString()) {
			delete[] data;
		}
	}

	/**/
	inline bool IsSmallString() const
	{
		return !(flags & 32);
	}

	/* Get as c string. Not a copy. */
	inline const char* CString() const
	{
		if (IsSmallString()) {
			return chars;
		}
		else {
			return data;
		}
	}

	/* Length of the string regardless of small or long. */
	inline uint64 Length() const 
	{
		if (IsSmallString()) {
			return SmallStringLength();
		}
		else {
			return LongStringLength();
		}
	}

	/* Amount of chars this string can hold. */
	inline uint64 Capacity() const 
	{
		if (IsSmallString()) {
			return MAX_SMALL_STRING_LENGTH + 1;
		}
		else {
			return capacity;
		}
	}

	/* std::cout << String */
	friend std::ostream& operator << (std::ostream& Os, String& _Str)
	{
		Os << _Str.CString();
		return Os;
	}

	/* Concatenate two strings into a new string. Does not overwrite any of the passed in string data. */
	static String ConcatenateStrings(const String& str1, const String& str2);

	/* Append the data of another string onto this one. If they two strings reference the same data,
	it'll simply copy the data, then append. */
	void AppendString(const String& Other);

	/* See AppendString(). */
	void operator += (const String& Other) {
		AppendString(Other);
	}

	/* See ConcatenateStrings(). */
	friend String operator + (const String& Left, const String& Right)
	{
		return String::ConcatenateStrings(Left, Right);
	}
};