GEN_SRC := RPCservice.cpp RPCserver_constants.cpp RPCserver_types.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))

SERVER=RPCservice_server
CLIENT=RPCservice_client


CXXFLAGS = -std=c++11 -Wall

.PHONY: all clean

all: $(SERVER) $(CLIENT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -I/usr/local/include/thrift -c $< -o $@

$(SERVER): $(SERVER:=.o) $(GEN_OBJ) 
	$(CXX) $^ -o $@  -lthrift -lcurl

$(CLIENT): $(CLIENT:=.o) $(GEN_OBJ)
	$(CXX) $^ -o $@  -lthrift 

clean:
	$(RM) *.o $(CLIENT) $(SERVER)