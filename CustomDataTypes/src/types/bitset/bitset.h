#pragma once

#include <type_traits>

#ifdef _DEBUG
#define BITSET_CHECK_VALID_INDEX
#endif

/*
		Bitset of variable specified bitsize. Bitset size of 0 bits not allowed (for obvious reasons).
		This implementation exists due to std::bitset having a minimum size of 4 bytes.
		This implementation allows much more compressed memory size.
		Memory format:
		- quantity <= 8: 1 byte (unsigned char)
		- quantity <= 16: 2 bytes (unsigned short)
		- quantity <= 32: 4 bytes (unsigned int)
		- quantity <= 64: 8 bytes	(unsigned long long)
		- quantity > 64: 16+ bytes (unsigned long long array)
*/
template<unsigned int bitQuantity>
struct bitset 
{
	static_assert(bitQuantity != 0, "bitset cannot have a quantity of 0");

	typedef std::conditional_t <bitQuantity <= 8, unsigned char,
			std::conditional_t <bitQuantity <= 16, unsigned short,
			std::conditional_t<bitQuantity <= 32, unsigned int, unsigned long long
			>>> Bittype;	
	
	/* Get the number of array elements held in this bitset. */
	constexpr static int GetArrayNum() { return (bitQuantity - 1) / 64 + 1; }

	/* Get the amount of bits this bitset stores. */
	constexpr static unsigned int GetAmountOfBits() { return bitQuantity; }

	union 
	{
		/* The bits. Also equivalent to bitsArray[0] due to union. */
		Bittype bits;

		/* The bits stores as an array with more than 1 elements if the amount of bits stored is greater than 64. */
		Bittype bitsArray[GetArrayNum()];
	};

	/* Default constructor. Initializes all flags to false. */
	bitset() 
	{
		for (int i = 0; i < GetArrayNum(); i++) {
			bitsArray[i] = 0;
		}
	}

	/* Constructor with a single set of flags. If bitset size is greater than 64, only the first array element is set, all others are set to 0. */
	bitset(Bittype initialFlags) 
	{
		bits = initialFlags;
		for (int i = 1; i < GetArrayNum(); i++) {
			bitsArray[i] = 0;
		}
	}

	/* Constructor with an array of flags. Array size must be equal to the array size of this bitset. */
	bitset(const Bittype other[GetArrayNum()]) 
	{
		for (int i = 0; i < GetArrayNum(); i++) {
			bitsArray[i] = other[i];
		}
	}

	/* Get the state of a bit at a specific index. Index can be greater than 64. If so, checks the further array elements. */
	bool GetBit(unsigned int index) 
	{
#ifdef BITSET_CHECK_VALID_INDEX
		if (index >= GetAmountOfBits()) return false;
#endif
		const int arrayIndex = index / 64;
		const int bitIndex = index % 64;
		return (bitsArray[arrayIndex] >> bitIndex) & 1;
	}

	/* Set the state of a bit at a specific index. Index can be greater than 64. If so, sets flag of further array elements. */
	void SetBit(unsigned int index, bool flag = true) 
	{
#ifdef BITSET_CHECK_VALID_INDEX
		if (index >= GetAmountOfBits()) return;
#endif
		const int arrayIndex = index / 64;
		const int bitIndex = index % 64;
		bitsArray[arrayIndex] ^= ((unsigned long long)(-flag) ^ bitsArray[arrayIndex]) & (1ULL << bitIndex);
	}

	/* Get a reference to an index of the bits array. Default index is 0. */
	Bittype& GetBitSubset(unsigned int subsetIndex = 0) 
	{
		return bitsArray[subsetIndex];
	}

	/**/
	void operator = (Bittype other) {
		bits = other;
	}
	
	/**/
	void operator = (const Bittype other[GetArrayNum()]) {
		for (int i = 0; i < GetArrayNum(); i++) {
			bitsArray[i] = other[i];
		}
	}

	/**/
	void operator = (const bitset<bitQuantity> other) {
		for (int i = 0; i < GetArrayNum(); i++) {
			bitsArray[i] = other.bitsArray[i];
		}
	}

	/* Boolean comparison of equality for another integer. */
	bool operator == (Bittype other) 
	{
		return bits == other;
	}

	/* Boolean comaprison of equality for another array of integers. */
	bool operator == (const Bittype other[GetArrayNum()]) 
	{
		for (int i = 0; i < GetArrayNum(); i++) {
			if (bitsArray[i] != other[i]) {
				return false;
			}
		}
		return true;
	}

	/* Boolean comparison of another bitset. */
	bool operator == (const bitset<bitQuantity> other) 
	{
		for (int i = 0; i < GetArrayNum(); i++) {
			if (bitsArray[i] != other.bitsArray[i]) {
				return false;
			}
		}
		return true;
	}

	/* Get the state of a bit at a specific index. Index can be greater than 64. If so, checks the further array elements. */
	bool operator [] (unsigned int index) {
		return GetBit(index);
	}
};

typedef bitset<8> bitset8;
typedef bitset<16> bitset16;
typedef bitset<32> bitset32;
typedef bitset<64> bitset64;