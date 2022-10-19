#pragma once

#include <types/array/DynamicArray.h>
#include <iostream>

typedef unsigned int uint;
constexpr size_t MAP_INITIAL_CAPACITY = 16;
constexpr double LOAD_FACTOR_FOR_RESIZE = 0.75;
constexpr ArrInt HUGE_BUCKET_SIZE = 8;

namespace MapHash {
	ArrInt Hash(int key);
}

template<typename K, typename V>
struct Pair 
{
	K key;
	V value;
};

template<typename K, typename V, ArrInt(*MapHasher)(K) = MapHash::Hash>
struct Map
{
	struct Bucket 
	{
		darray<Pair<K, V>> elements;

		V* Find(const K& key) {
			for (ArrInt i = 0; i < elements.Size(); i++) {
				if (elements[i].key == key) {
					return &elements[i].value;
				}
			}
			return nullptr;
		}
	};

	//Array<Bucket> buckets;

	Bucket* buckets;

	ArrInt bucketCount;

	ArrInt elementCount;

	constexpr Map() {
		//bucketCount = MAP_INITIAL_CAPACITY;
		//bucketsalt = MakeNewBucketArray(bucketCount);

		//buckets.Reserve(MAP_INITIAL_CAPACITY);
		//buckets.FillWith(Bucket());
		//elementCount = 0;
	}

	constexpr ~Map() {
		delete[] buckets;
	}

	static ArrInt ComputeHash(const K& key) {
		return MapHasher(key);
	}

	static ArrInt GetBucketForKey(ArrInt bucketCount, const K& key) {
		const ArrInt hash = ComputeHash(key);
		const ArrInt index = hash % bucketCount;
		return index;
	}

	void Add(const Pair<K, V>& keyValuePair) 
	{
		CheckIfMapShouldGrow();

		const ArrInt bucketIndex = GetBucketForKey(bucketCount, keyValuePair.key);
		buckets[bucketIndex].elements.Add(keyValuePair);
		elementCount++;
	}

	void Add(const K& key, const K& value) 
	{
		Add({ key, value });
	}

	void CheckIfMapShouldGrow() 
	{
		double amountOfBucketsWithStuff = 0;
		for (ArrInt i = 0; i < bucketCount; i++) {
			if (buckets[i].elements.Size() > 0) {
				amountOfBucketsWithStuff++;
			}

			if (buckets[i].elements.Size() > HUGE_BUCKET_SIZE) {
				GrowMap();
				return;
			}
		}

		const bool mapShouldGrow = (amountOfBucketsWithStuff / double(bucketCount)) > LOAD_FACTOR_FOR_RESIZE;
		if (mapShouldGrow)
			GrowMap();
	}

	void GrowMap() 
	{
		darray<Bucket> newBuckets;

		const ArrInt newBucketCount = _ArrayCapacityIncrease(bucketCount);
		newBuckets.Reserve(newBucketCount);
		newBuckets.FillWith(Bucket());

		for (ArrInt b = 0; b < buckets.Size(); b++) {
			for (ArrInt p = 0; p < buckets[b].elements.Size(); p++) {

				const Pair<K, V>& keyValuePair = buckets[b].elements[p];
				const ArrInt bucketIndex = GetBucketForKey(newBuckets.Size(), keyValuePair.key);
				newBuckets[bucketIndex].elements.Add(keyValuePair);
			}
		}

		buckets = newBuckets;
	}

	static constexpr Bucket* MakeNewBucketArray(const ArrInt NewBucketCount)
	{
		Bucket* newBuckets = new Bucket[NewBucketCount];
		for (ArrInt i = 0; i < NewBucketCount; i++) {
			newBuckets[i] = Bucket();
		}
		return newBuckets;
	}
};

