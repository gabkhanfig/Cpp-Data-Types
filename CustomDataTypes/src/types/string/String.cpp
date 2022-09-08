#include "String.h"

String String::ConcatenateStrings(const String& Str1, const String& Str2)
{
	const uint64 NewSize = Str1.Length() + Str2.Length();
	const uint64 NewCapacity = NewSize + 1;
	
	const char* Str1Data = Str1.CString();
	const char* Str2Data = Str2.CString();

	String _String;
	_String.SetLength(NewSize);
	if (_String.IsSmallString()) {
		memcpy(_String.chars, Str1Data, Str1.SmallStringLength());
		memcpy(&_String.chars[Str1.SmallStringLength()], Str2Data, Str2.SmallStringLength() + 1);
	}
	else {
		char* NewData = new char[NewCapacity];
		memcpy(NewData, Str1Data, Str1.Length());
		memcpy(&NewData[Str1.Length()], Str2Data, Str2.Length() + 1);
		_String.data = NewData;
		_String.capacity = NewCapacity;
	}
	return _String;
}

void String::AppendString(const String& Other)
{
	const uint64 NewSize = Length() + Other.Length();
	if (NewSize <= MAX_SMALL_STRING_LENGTH) {
		memcpy(&chars[Length()], Other.chars, Other.SmallStringLength() + 1);
		SetLengthSmall(NewSize);
	}
	else {
		const uint64 NewMinCapacity = NewSize + 1;
		const uint64 InitialLength = Length();
		if (IsSmallString() || NewMinCapacity > capacity) {
			IncreaseLongStringCapacity(NewMinCapacity);
		}

		const char* otherStr = Other.CString();
		memcpy(&data[InitialLength], otherStr, Other.Length() + 1);
		SetLengthLong(NewSize);
	}
}

void String::IncreaseLongStringCapacity(uint64 MinCapacity)
{
	const uint64 NewCapacity = 3 * (MinCapacity) >> 1;
	char* NewData = new char[NewCapacity];
	const char* OldData = CString();
	memcpy(NewData, OldData, Length() + 1);
	if (!IsSmallString()) {
		delete[] data;
	}
	data = NewData;
	capacity = NewCapacity;
	SetLengthLong(Length());
	SetIsLongString();
}
