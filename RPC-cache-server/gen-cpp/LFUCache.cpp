//LFUCache.cpp
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "gtcache.h"

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <stack>


// Vector to store cache entries -- vector size can be reserved once cache size is known
static std::vector<cache_entry_t> cache;

// Hashtable(implemented via unordered map O(1) lookup) stores the url vs the cache entry id + number of hits pair
static std::unordered_map<std::string, std::pair<int, int> > hashTable;       

// stack(O(1) pop) to store available ids
static std::stack<int> available_ids;

//Priority queue(implemented via ordered map O(1) quick and O(logN) average lookup) to map url vs minimum number of hits
//Key consists of a pair of number of hits and cache location to enable direct access to the hit/url pair
static std::map< std::pair<int, int>, bool > eviction_queue;

void deleteEntryFromCache(int pos){
  /*
    e is assumed to have already been evicted from the priority queue.
  */
   if (pos >= max_cache_size){
   	printf("Cannot delete entry from cache: invalid location\n");
   }
  cache_entry_t * delEntry = &cache[pos];
  used_mem -= delEntry->val_size > min_size ? delEntry->val_size : min_size;
  delEntry->hits = 0;
  hashTable.erase(delEntry->url);

  free(delEntry->content);

  available_ids.push(delEntry->id);
}

cache_entry_t* createEntryInCache(std::string url, void* value, size_t val_size){
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
  e->hits += 1;
  memcpy(e->content, value, val_size);
  hashTable[e->url] = std::make_pair(e->hits, e->id);
  eviction_queue[std::make_pair(e->hits, e->id)] = true;
  printf("Entering into eviction queue hits %d id %d\n", e->hits, e->id);
  //std::cout << eviction_queue[std::make_pair(e->hits, e->id)] << std::endl;
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

/*
 * The gtcache_set procedure associates the first val_size bytes after
 * the pointer value with the key.  A defensive copy of the data is made.
 */
 int gtcache_set(std::string key, void* value, size_t val_size){
 	
 	std::pair<int, int> location;
 	if (val_size > cache_capacity){
		/* It's hopeless. */
 		fprintf(stderr, "Value exceeds cache limit\n");
 		return 1;
 	}

	//find url in the hashtable

 	std::unordered_map<std::string, std::pair<int, int> >::const_iterator got = hashTable.find(key);

 	if ( got == hashTable.end() ){
	/* Now that we know it isn't already in cache, let's see if there is room */
 		std::cout << "The entry not in the cache, make room for it...";
 		while(used_mem + val_size > cache_capacity){
 			std::map< std::pair<int, int>, bool >::iterator pq = eviction_queue.begin();
			deleteEntryFromCache(pq->first.second); //cache position
			eviction_queue.erase(pq);
		}
		if( createEntryInCache(key, value, val_size) == NULL){
			fprintf(stderr, "gtcache_set: unable to create cache_entry\n");
			return -1;
		}

	}
	else{
		/* If it is already in the cache...
		   find the cache location */
		printf("Found in hashmap..the key and location..\n");
		location = got->second;
		int cache_index = location.second;
		//update cache
		if (cache[cache_index].val_size == val_size){
			memcpy(cache[cache_index].content, value, val_size);
			cache[cache_index].hits += 1;	
		}
		//update priority queue
		std::map< std::pair<int, int>, bool >::iterator pq = eviction_queue.find(got->second);
		
		if (pq == eviction_queue.end()){
			std::cout << "Not found in priority queue";
		}
		else{
			std::pair<int,int> pq_location = pq->first;
			eviction_queue.erase(pq);
			pq_location.first += 1;
			eviction_queue[pq_location] = true;
			location.first = pq_location.first;
			hashTable[key] = location;
		
		}
	
	}
	return 0;
}

/*
 * The gtcache_get procedure returns a copy of the data associated
 * with the key.  If the parameter val_size is NULL, it is ignored.
 * Otherwise, the length of the data array is stored in it.
 */

void * gtcache_get(std::string key, size_t* val_size){
	
	std::pair<int, int> location;
	void *ans;

	std::unordered_map<std::string, std::pair<int, int> >::const_iterator got = hashTable.find(key);

	// Cache Miss -- URL not in cache
	if (got == hashTable.end()){
		return NULL;
	}
	
	/* Mark e as used, if necessary */
	location = got->second;
	//printf("Finding in priority queue hits %d id %d\n", got->second.first, got->second.second);
	int cache_index = location.second;
	
	cache[cache_index].hits +=  1;
	printf("Finding in priority queue hits %d id %d\n", got->second.first, got->second.second);
	//update priority queue
	std::map< std::pair<int, int>, bool >::iterator pq = eviction_queue.find(location);
	if (pq == eviction_queue.end()){
		std::cout << "Error : URL Not found in priority queue";
	}
	else{
		std::pair<int,int> pq_location = pq->first;
		eviction_queue.erase(pq);
		pq_location.first += 1;
		eviction_queue[pq_location] = true;
		location.first = pq_location.first;
		hashTable[key] = location;
	}
	
	if( val_size != NULL)
		*val_size = cache[cache_index].val_size;

	ans = malloc(cache[cache_index].val_size);
	memcpy(ans, cache[cache_index].content, cache[cache_index].val_size);

	return ans;
}

/* 
 * The gtcache_memused procedure returns the sum of the sizes of the
 * entries in the cache.  This amount should never exceed capacity.
 */
int gtcache_memused(){
	return used_mem;
}

/*
 * The gtcache_destroy procedure cleans up the memory created for the
 * cache.  The procedure gtcache_init must be called before the cache
 * can be used again.
 */
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
