// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "RPCservice.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <stdlib.h>
#include "gtcache.h"
#include <curl/curl.h>
#include <rpc/rpc.h>
#include <sys/dir.h>
#include <time.h>
#include <sys/time.h>
#include <ctime>
#include <fstream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::RPC;

struct MemoryStruct {
  char *memory;
  size_t size;
};
  
void log(std::string stringToWrite, std::string fileName) {
  std::ofstream outfile;
  outfile.open(fileName, std::ios_base::app);
  outfile << stringToWrite;
  return;
}
 
std::string file_hit = "log_lfu_random_large_hit_8.txt";
std::string file_time = "log_lfu_random_large_time_8.txt";
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
  ///printf("in writememoryblah.\n");
  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
  //printf("brefore return.\n");
  return realsize;
}
 

void get_by_curl(const std::string &url, struct MemoryStruct* chunk){
  CURL *curl_handle;
  CURLcode res;
  
  chunk->memory = (char *)malloc(1024);
  //chunk->memory = "";  /* will be grown as needed by the realloc above */ 
  chunk->size = 0;    /* no data at this point */ 
 // printf("Getting curl.\n");
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 //printf("After callback.\n");
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 //printf("After libcurl.\n");
  /* get it! */ 
res = curl_easy_perform(curl_handle);
 //printf("After agent.\n");
  /* check for errors */ 
  if(res != CURLE_OK) {
    std::fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    throw -10;
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();

  return;
}

class RPCserviceHandler : virtual public RPCserviceIf {
 public:
  RPCserviceHandler() {
    // Your initialization goes here
    gtcache_init(WWW_MIN_CACHE_LEN * CACHE_SIZE, WWW_MIN_CACHE_LEN, 1);
  }

  void getWebPage(responseData& _return, const std::string& url, int32_t size) {
    // Your implementation goes here
    struct MemoryStruct chunk;
    size_t rand_size = size;
    clock_t start, end;

    try {
  //    printf("Enter try.\n");
      char *pageContent = (char *) malloc(2048);
      start = clock();
      pageContent = (char *) gtcache_get(url, &rand_size);
      end = clock();
      if (size == 0){
        size = rand_size;
      }
      //if MISS
      if (pageContent == NULL || rand_size < size){
        get_by_curl(url, &chunk);
        end = clock();
        printf("A possible Miss for %s...\n", url.c_str());
  
        _return.isSuccess = true;
        _return.webPage = std::string(chunk.memory, chunk.memory + chunk.size); 
        if (size == 0){
          size = chunk.size;
        }
        int isAhit = gtcache_set(url, chunk.memory, size);
        log("Miss\n", file_hit);
        /*if (isAhit == 2){
          printf("A hit for %s\n", url.c_str());
          log("Hit\n", file_hit);
        }
        else{
          printf("A Miss for %s\n", url.c_str());
          log("Miss\n", file_hit);
        } */
      }
      else{
        // A hit!
        printf("A hit for %s\n", url.c_str());
        _return.isSuccess = true;
        _return.webPage = std::string(pageContent, pageContent + size);
        log("Hit\n", file_hit);
      }
      printf("size of page %d\n", size);
      
    }
    catch(...){
      _return.isSuccess = false;
      printf("An exception occurred.\n");

    }
    time_t current_time = time(NULL);
    printf("Server completed the request in %lu microseconds on %s \n", (end-start)*(1000000/CLOCKS_PER_SEC), ctime(&current_time));
    std::string t = std::to_string((end-start)*(1000000/CLOCKS_PER_SEC));
    log(t + "\n", file_time);
  }

  ~RPCserviceHandler() {
    gtcache_destroy();
  }
};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<RPCserviceHandler> handler(new RPCserviceHandler());
  shared_ptr<TProcessor> processor(new RPCserviceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

