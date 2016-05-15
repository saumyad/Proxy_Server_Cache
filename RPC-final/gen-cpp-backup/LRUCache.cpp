//LFUCache.cpp
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <ctime>
#include <string.h>
#include "gtcache.h"

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <stack>

long x = -1;
int y = -1;

// Vector to store cache entries -- vector size can be reserved once cache size is known
static std::vector<cache_entry_t> cache;

// Hashtable(implemented via unordered map O(1) lookup) stores the url vs the cache entry id + number of hits pair
static std::unordered_map<std::string, std::pair<long, int> > hashTable;       

// stack(O(1) pop) to store available ids
static std::stack<int> available_ids;

//Priority queue(implemented via ordered map O(1) quick and O(logN) average lookup) to map url vs minimum number of hits
//Key consists of a pair of number of hits and cache location to enable direct access to the hit/url pair
static std::map< std::pair<long, int>, bool > eviction_queue;

std::pair<long, int> findInCache(std::string url){
	std::unordered_map<std::string, std::pair<long, int> >::const_iterator got = hashTable.find(url);

	// Cache Miss -- URL not in cache
	if (got == hashTable.end()){
		return std::make_pair(x, y);
	}
	return got->second;
} 

std::pair<long, int> findInPriorityQueue(std::pair<long, int> location){
	std::map< std::pair<long, int>, bool >::iterator pq = eviction_queue.find(location);
	if (pq == eviction_queue.end()){
		std::cout << "Warning : URL Not found in priority queue";
		return std::make_pair(x, y);
	}
	return pq->first;
}

void deleteEntryFromCache(int pos){
	/*
	e has already been evicted from the priority queue.
	*/
	if (pos >= max_cache_size){
		printf("Cannot delete entry from cache: invalid location\n");
	}
	printf("Evicting %d with url %s from cache\n", pos, cache[pos].url.c_str());
	cache_entry_t * delEntry = &cache[pos];
	used_mem = used_mem - delEntry->val_size > 0 ? used_mem - delEntry->val_size : 0;
	delEntry->hits = 0;
	hashTable.erase(delEntry->url);
	free(delEntry->content);
	printf("Memory used up now %d\n", gtcache_memused());
	available_ids.push(delEntry->id);
}

cache_entry_t* createEntryInCache(std::string url, void* value, size_t val_size){
	struct timeval entry;
	clock_t start;
	cache_entry_t* e;

	if(available_ids.size() == 0){
		fprintf(stderr, "ran out of ids for cache entries!\n");
		return NULL;
	}

	used_mem += val_size;
	int evict = available_ids.top();
	available_ids.pop(); 
	e = &cache[evict];
	e->id = evict;
	e->url = url;
	e->content = (void*) malloc(val_size);
	e->val_size = val_size;
	gettimeofday(&entry, NULL);
	start = clock();
	long currTime = (long) start;
		
	e->hits = currTime;
	memcpy(e->content, value, val_size);
	hashTable[e->url] = std::make_pair(e->hits, e->id);
	eviction_queue[std::make_pair(e->hits, e->id)] = true;
	printf("Creating entry in cache with id %d\n", e->id);
	printf("Memory used up in cache %d\n", gtcache_memused());
	return e;
}

int gtcache_init(size_t capacity, size_t min_entry_size, int num_levels){

	min_size = min_entry_size;
	cache_capacity = capacity;
	used_mem = 0;

	max_cache_size = cache_capacity / min_size;

	// initiate cache size
	cache.reserve(max_cache_size);

	// populate stack with available id's  
	for( int j = 0; j < max_cache_size; j++){
		cache_entry_t cache_entry;
		cache_entry.url = "";
		cache_entry.id = j;
		cache_entry.hits = 0;
		cache.push_back(cache_entry);
		available_ids.push(j);
	}
	return 0;
}

int gtcache_set(std::string key, void* value, size_t val_size){
	struct timeval entry;
	clock_t start;
	std::pair<long, int> location, pq_location;
	if (val_size > cache_capacity){
		fprintf(stderr, "Value exceeds cache limit\n");
		return 1;
	}

	//find url in the hashtable
	location = findInCache(key);

	if (location != std::make_pair(x, y)){
		printf("Entry already found in cache..checking if data is enough..\n");
		/* If it is already in the cache...find the cache location */
		int cache_index = location.second;

		gettimeofday(&entry, NULL);
		start = clock();
		long currTime = (long) start;
		//Update Priority Queue
		pq_location = findInPriorityQueue(location);
		
		//Update Cache
		if (cache[cache_index].val_size == val_size){
			printf("Data is enough..Infact a hit %d %d\n", cache[cache_index].val_size, val_size);
			memcpy(cache[cache_index].content, value, val_size);
			cache[cache_index].hits = currTime;	

			if (pq_location == std::make_pair(x, y)){
			/* Inconsistency */
			std::cout << "";
			}
			else{
				eviction_queue.erase(pq_location);
				pq_location.first = currTime;
				eviction_queue[pq_location] = true;
				location.first = currTime;
				hashTable[key] = location;
			}
			return 2;
		}
		else{
			printf("Data stale/not enough. Removing from the cache..\n");
			if (pq_location == std::make_pair(x, y)){
				std::cout << "Not found in priority queue";
			}
			else{
				// delete stale entry
				eviction_queue.erase(pq_location);
				hashTable.erase(key);
			}
		}		
		
	}
	
		/* Now that we know it isn't already in cache, let's see if there is room */
		std::cout << "The entry not in the cache, make room for it...";
		while(used_mem + val_size > cache_capacity || available_ids.size() == 0){
			std::map< std::pair<long, int>, bool >::iterator pq = eviction_queue.begin();
			deleteEntryFromCache(pq->first.second); //cache position
			eviction_queue.erase(pq);
		}
		if(createEntryInCache(key, value, val_size) == NULL){
			fprintf(stderr, "gtcache_set: Unable to create cache_entry as number of entries reach their limit\n");
			return -1;
		}
	
	return 0;
}


void * gtcache_get(std::string key, size_t* val_size){
	struct timeval entry;
	clock_t start;
	std::pair<long, int> location, pq_location;
	void *ans;

	location = findInCache(key);
	if (location == std::make_pair(x, y)){
		/* Cache Miss */
		return NULL;
	}

	gettimeofday(&entry, NULL);		
	start = clock();
	long currTime = (long) start;
		
	/* Update Cache */
	int cache_index = location.second;
	cache[cache_index].hits = currTime;
	
	/* Update Priority Queue */
	printf("Finding in priority queue: Time of entry %ld, Id %d\n", location.first, location.second);
	
	pq_location = findInPriorityQueue(location);	
	if (pq_location == std::make_pair(x, y)){
		/* Inconsistency */
		std::cout << "Q" << std::endl;
	}
	else{
		eviction_queue.erase(pq_location);
		pq_location.first = currTime;
		eviction_queue[pq_location] = true;
		/* Update Hash Table */
		location.first = currTime;
		hashTable[key] = location;
	}

	if( val_size != NULL){
		*val_size = cache[cache_index].val_size;
	}

	ans = malloc(cache[cache_index].val_size);
	memcpy(ans, cache[cache_index].content, cache[cache_index].val_size);
	return ans;
}

int gtcache_memused(){
	return used_mem;
}

void gtcache_destroy(){
	eviction_queue.clear();
	hashTable.clear();
	cache.clear();

	while (!available_ids.empty())
	{
		available_ids.pop();
	}
	return;	
}
