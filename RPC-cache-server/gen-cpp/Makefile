GEN_SRC := RPCservice.cpp RPCserver_constants.cpp RPCserver_types.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))
LDFLAGS = -L/usr/local/Cellar/curl/7.47.1/lib -lldap -lz
CPPFLAGS = -I/usr/local/Cellar/curl/7.47.1/include
CURL_CFLAGS = $(shell curl-config --cflags)

SERVER=RPCservice_server
CLIENT=RPCservice_client
CACHE_SRC=LRUCache.cpp

CXXFLAGS = -std=c++11 -Wall


all: $(SERVER) $(CLIENT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -I/usr/local/include/thrift  -c $< -o $@

$(SERVER): $(SERVER:=.o) $(GEN_OBJ) $(CACHE_SRC:.cpp=.o)
	$(CXX) $^ -o $@  $(CPPFLAGS) $(CURL_CFLAGS) -lthrift -lcurl $(LDFLAGS)

$(CLIENT): $(CLIENT:=.o) $(GEN_OBJ)
	$(CXX) $^ -o $@  -lthrift 

clean:
	$(RM) *.o $(CLIENT) $(SERVER)