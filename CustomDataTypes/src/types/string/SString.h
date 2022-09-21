#pragma once

#include <iostream>

#define MAX_SMALL_SSTRING_LENGTH			sizeof(char*) + sizeof(size_t) - 1
#define SSTRING_SMALL_FLAG_BIT				0b10000000
#define SSTRING_SMALL_OTHER_BITS			0b01111111
#define SSTRING_LONG_LENGTH_BITMASK		0b0111111111111111111111111111111111111111111111111111111111111111UL

struct SString 
{
private:

	/**/
	union 
	{
		struct 
		{
			char* data;
			size_t length;
		};
		struct 
		{
			char sso_chars[MAX_SMALL_SSTRING_LENGTH + 1];
		};
	};

	void SetIsSmallString() {
		sso_chars[MAX_SMALL_SSTRING_LENGTH] &= ~(1 << 7);
	}

	void SetIsLongString() {
		sso_chars[MAX_SMALL_SSTRING_LENGTH] |= 1 << 7;
	}

	__forceinline void SetLongLength(size_t new_length) {
		length = new_length;
		SetIsLongString();
	}

	__forceinline void SetSmallLength(size_t new_length) {
		sso_chars[MAX_SMALL_SSTRING_LENGTH] = MAX_SMALL_SSTRING_LENGTH - new_length;
		SetIsSmallString();
	}

	__forceinline size_t GetSmallStringLength() const {
		return MAX_SMALL_SSTRING_LENGTH - (sso_chars[MAX_SMALL_SSTRING_LENGTH] & SSTRING_SMALL_OTHER_BITS);
	}

	__forceinline size_t GetLongStringLength() const {
		return length & SSTRING_LONG_LENGTH_BITMASK;
	}

	void SetLength(size_t new_length) {
		if (new_length > MAX_SMALL_SSTRING_LENGTH) {
			SetLongLength(new_length);
		}
		else {
			SetSmallLength(new_length);
		}
	}

public:
	
	SString() {
		sso_chars[0] = '\0';
		SetSmallLength(0);
	}

	SString(const char* str) {
		const size_t slen = strlen(str);
		SetLength(slen);
		if (IsSmallString()) {
			memcpy(sso_chars, str, slen + 1);
		}
		else {
			data = new char[slen + 1];
			memcpy(data, str, slen + 1);
		}
	}

	SString(const SString& other) {
		const size_t slen = other.Length();
		SetLength(slen);
		if (IsSmallString()) {
			memcpy(sso_chars, other.CStr(), slen + 1);
		}
		else {
			data = new char[slen + 1];
			memcpy(data, other.CStr(), slen + 1);
		}
	}

	~SString() {
		if (!IsSmallString()) {
			delete[] data;
		}
	}

	inline size_t Length() const {
		const bool sso = IsSmallString();
		return (sso * GetSmallStringLength()) + (!sso * GetLongStringLength());
	}

	bool IsSmallString() const {
		return !(sso_chars[MAX_SMALL_SSTRING_LENGTH] & SSTRING_SMALL_FLAG_BIT);
	}

	const char* CStr() const  {
		if (IsSmallString()) {
			return sso_chars;
		}
		else {
			return data;
		}
	}

	/* std::cout << SString */
	friend std::ostream& operator << (std::ostream& os, SString& str)
	{
		os << str.CStr();
		return os;
	}

	void operator = (const char* str) {
		if (!IsSmallString() && data) delete[] data;

		const size_t slen = strlen(str);
		SetLength(slen);
		if (IsSmallString()) {
			memcpy(sso_chars, str, slen + 1);
		}
		else {
			data = new char[slen + 1];
			memcpy(data, str, slen + 1);
		}
	}

	void operator = (const SString& other) {
		if (!IsSmallString() && data) delete[] data;

		const size_t slen = other.Length();
		SetLength(slen);
		if (IsSmallString()) {
			memcpy(sso_chars, other.CStr(), slen + 1);
		}
		else {
			data = new char[slen + 1];
			memcpy(data, other.CStr(), slen + 1);
		}
	}

	bool operator == (const char* str) {
		return strcmp(CStr(), str) == 0;
	}

	bool operator == (const SString& other) {
		if (Length() != other.Length()) return false;
		return strcmp(CStr(), other.CStr()) == 0;
	}

	static SString Concatenate(const SString& left, const SString& right);

	void Append(const char* str);

	void Append(const SString& other);

	friend SString operator + (const SString& left, const SString& right) {
		return SString::Concatenate(left, right);
	}

	void operator += (const char* str) {
		Append(str);
	}

	void operator += (const SString& other) {
		Append(other);
	}
};