#pragma once

#include <types/array/Array.h>

typedef unsigned int uint;
constexpr size_t MAP_INITIAL_CAPACITY = 10;

template<typename K, typename V>
struct Map 
{

public:

	/* Key-value pair. */
	struct map_pair 
	{
		K key;
		V value;
	};

	struct map_bucket 
	{
		static const size_t bucket_default_capacity = 2;
		Array<map_pair> bucket;

		map_bucket()
		{
			bucket.Reserve(bucket_default_capacity);
		}
	};

	/* Array of buckets. */
	Array<map_bucket*> data;

	/* Amount of key value pairs held in the map. */
	size_t count;

	/* Amount of buckets this map can hold. */
	size_t capacity;

	Map() 
	{
		std::cout << "map constructor\n";
		data.Reserve(MAP_INITIAL_CAPACITY);
		for (size_t i = 0; i < MAP_INITIAL_CAPACITY; i++) {
			data.Add(new map_bucket());
		}
	}

	~Map() 
	{
		for (auto& bucket : data) {
			delete bucket;
		}
	}

};