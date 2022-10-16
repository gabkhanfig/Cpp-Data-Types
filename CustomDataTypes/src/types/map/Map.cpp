#include "Map.h"

ArrInt MapHash::Hash(int key)
{
	return key;
}

constexpr ArrInt SomeHash(int key) {
	return key;
}

consteval bool Test() {
	//constexpr Map<int, float, SomeHash> m;
	return true;
}

static_assert(Test(), "a");
