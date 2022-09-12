#pragma once

#include <iostream>
#include <types/array/Array.h>

typedef unsigned char uint8;
typedef unsigned long long uint64;

#define MAX_SMALL_STRING_LENGTH			31
#define STRING_FLAG_SMALL_SIZE_BITS 0b00011111
#define STRING_FLAG_IS_LONG					0b00100000
#define STRING_FLAG_UNUSED					0b11000000

/* Dynamically changing string, with small string optimization for strings of length 31 (excluding null terminator).
Occupies 32 bytes, same as msvc std::string, conviniently is a power of 2 to (hopefully) place multiple strings cleanly in cache lines.
Supremely outperforms msvc std::string in small string instantiation and concatenation,
and fairly outperforms std::string in long string instantiation and concatenation. */
struct String 
{
public:

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

private:

	/**/
	inline void SetIsSmallString() 
	{
		flags = flags & (STRING_FLAG_UNUSED + STRING_FLAG_SMALL_SIZE_BITS);
	}

	/**/
	inline void SetIsLongString() 
	{
		flags = flags | STRING_FLAG_IS_LONG;
	}

	/* Small string length is stored as an offset from the max length. */
	inline uint64 SmallStringLength() const
	{
		return MAX_SMALL_STRING_LENGTH - (flags & STRING_FLAG_SMALL_SIZE_BITS);
	}

	/**/
	inline uint64 LongStringLength() const
	{
		return length;
	}

	/**/
	inline void SetLengthSmall(uint8 SmallLength)
	{
		flags = (MAX_SMALL_STRING_LENGTH - SmallLength) + (flags & STRING_FLAG_UNUSED);
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

	/* Whether this string is currently using the small string implementation. */
	inline bool IsSmallString() const
	{
		return !(flags & STRING_FLAG_IS_LONG);
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

	/* Equivalency. Only checks raw string data. */
	bool operator == (const char* Str) const {
		return strcmp(CString(), Str) == 0;
	}

	/* Equivalency. */
	bool operator == (const String& Other) const {
		if (Length() != Other.Length()) return false;
		return strcmp(CString(), Other.CString()) == 0;
	}

	/* Set equal to a const char* string. If the string is small enough to be SSO'd, it will be. Copies the data. */
	void operator = (const char* _String) 
	{
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

	/* Set equal to another string, copying the data. */
	void operator = (const String& Other) 
	{
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

	/* Get a copy of a character at a specific index. */
	char GetCharAt(const uint64 index) {
		if (index >= Length()) return '\0';

		return CString()[index];
	}

	/* Get a copy of the character at a specific index. */
	char operator [] (const uint64 index) {
		return GetCharAt(index);
	}

	/* Get a copy of a substring from a boundary.
	@param start: character index (included).
	@param end: character index (excluded). */
	String Substring(uint64 start, uint64 end);
	
	/* Split string into a copy of array of strings given a character splitter. */
	Array<String> Split(char splitter);

	/* Split string into a copy of array of strings given a string splitter. */
	Array<String> Split(const String& splitter);

};