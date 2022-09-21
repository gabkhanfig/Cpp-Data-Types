#include "SString.h"

SString SString::Concatenate(const SString& left, const SString& right)
{
	const size_t slen = left.Length() + right.Length();

	const char* Str1Data = left.CStr();
	const char* Str2Data = right.CStr();

	SString str;
	str.SetLength(slen);
	if (str.IsSmallString()) {
		memcpy(str.sso_chars, Str1Data, left.GetSmallStringLength());
		memcpy(&str.sso_chars[left.GetSmallStringLength()], Str2Data, right.GetSmallStringLength() + 1);
	}
	else {
		char* NewData = new char[slen + 1];
		memcpy(NewData, Str1Data, left.Length());
		memcpy(&NewData[left.Length()], Str2Data, right.Length() + 1);
		str.data = NewData;
	}

	return str;
}

void SString::Append(const char* str)
{
	const size_t slen = strlen(str);
	const size_t currentlen = Length();
	if (slen + currentlen <= MAX_SMALL_SSTRING_LENGTH) {
		memcpy(&sso_chars[currentlen], str, slen + 1);
		SetSmallLength(slen + currentlen);
	}
	else {
		char* newData = new char[slen + currentlen + 1];
		memcpy(newData, CStr(), currentlen);
		memcpy(&newData[currentlen], str, slen + 1);
		if (!IsSmallString()) {
			delete[] data;
		}
		data = newData;
		SetLongLength(slen + currentlen);
	}
}

void SString::Append(const SString& other)
{
	const size_t slen = other.Length();
	const size_t currentlen = Length();
	if (slen + currentlen <= MAX_SMALL_SSTRING_LENGTH) {
		memcpy(&sso_chars[currentlen], other.sso_chars, slen + 1);
		SetSmallLength(slen + currentlen);
	}
	else {
		char* newData = new char[slen + currentlen + 1];
		memcpy(newData, CStr(), currentlen);
		memcpy(&newData[currentlen], other.CStr(), slen + 1);
		if (!IsSmallString()) {
			delete[] data;
		}
		data = newData;
		SetLongLength(slen + currentlen);
	}
}
