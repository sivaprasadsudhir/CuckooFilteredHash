#include "cuckoofilter.h"

#include <iostream>
#include <libcuckoo/cuckoohash_map.hh>

#include <assert.h>
#include <math.h>
#include <vector>

using cuckoofilter::CuckooFilter;
using namespace std;

class FilteredCuckooHash {
public:
	CuckooFilter<int, 12> filter;
	cuckoohash_map<int, int> hashmap;

	FilteredCuckooHash(int total_items): filter(total_items) {
		hashmap.reserve(total_items);
	}

	bool find(const int &key, int &val) const {
		if(filter.Contain(key) != cuckoofilter::Ok)
			return false;
		return hashmap.find(key, val);
	}

	bool contains(const int &key) const {
		if(filter.Contain(key) != cuckoofilter::Ok)
			return false;
		return hashmap.contains(key);
	}

	bool insert(int &key, int &val) {
		if(filter.Add(key) != cuckoofilter::Ok)
			return false;
		if(hashmap.insert(key, val))
			return true;
		if(filter.Delete(key) == cuckoofilter::Ok)
			return false;
		// Should not reach here, throw an exception
		return false;
	}

	bool erase(const int &key) {
		if(filter.Delete(key) != cuckoofilter::Ok)
			return false;
		if(hashmap.erase(key))
			return true;
		if(filter.Add(key) == cuckoofilter::Ok)
			return false;
		// Should not reach here, throw an exception
		return false;
	}
};

int main(int argc, char **argv) {
	int total_items = 1000000;

	FilteredCuckooHash filteredhash(total_items);

	// Insert items to this filtered hash table
	int num_inserted = 0;
	for (int i = 0; i < total_items; i++, num_inserted++) {
		int val = -i;
		if (!filteredhash.insert(i, val)) {
			break;
		}
	}

	std::cout << "Successfully inserted: " << num_inserted << std::endl;

	// Check if previously inserted items are in the filter, expected
	// true for all items
	for (int i = 0; i < num_inserted; i++) {
		assert(filteredhash.contains(i));
	}

	// Check if previously inserted items are in the table, expected
	// true for all items and val == -i
	for (int i = 0; i < num_inserted; i++) {
		int val;
		assert(filteredhash.find(i, val));
		assert(val == -i);
	}

	// Check non-existing items, no false positives expected
	for (int i = total_items; i < 2 * total_items; i++) {
		assert(!filteredhash.contains(i));
	}

	// Delete all the values, true expected
	// find should return false
	// contains should return false
	for (int i = 0; i < num_inserted; i++) {
		assert(filteredhash.erase(i));
		int val;
		assert(!filteredhash.find(i, val));
		assert(!filteredhash.contains(i));
	}

	std::cout << "Test Successful" << std::endl;

	return 0;
}
