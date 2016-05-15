// RPCservice server file

#include "RPCservice.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <stdlib.h>

#include <curl/curl.h>
#include <rpc/rpc.h>
#include <sys/dir.h>
#include <time.h>

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
 
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
  printf("in writememoryblah.\n");
  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 printf("brefore return.\n");
  return realsize;
}
 

void get_by_curl(const std::string &url, struct MemoryStruct* chunk){
  CURL *curl_handle;
  CURLcode res;
  
  chunk->memory = (char *)malloc(512);
  //chunk->memory = "";  /* will be grown as needed by the realloc above */ 
  chunk->size = 0;    /* no data at this point */ 
  printf("Getting curl.\n");
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 printf("After callback.\n");
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 printf("After libcurl.\n");
  /* get it! */ 
res = curl_easy_perform(curl_handle);
 printf("After agent.\n");
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
  }

  void getWebPage(responseData& _return, const std::string& url) {
    // Your implementation goes here
    struct MemoryStruct chunk;
  //  memset(&chunk, 0, sizeof(chunk));
    try {
      printf("Enter try.\n");
      get_by_curl(url, &chunk);
      printf("Exit get curl.\n");
      _return.isSuccess = true;
      _return.webPage = std::string(chunk.memory, chunk.memory + chunk.size);
    }
    catch(...){
      _return.isSuccess = false;
      printf("An exception occurred.\n");

    }
    time_t current_time = time(NULL);
    printf("Server completed the request at %s\n", ctime(&current_time));
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

