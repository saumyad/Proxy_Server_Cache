/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <iostream>
#include <fstream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "RPCservice.h"
#include "gtcache.h"
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace RPC;


int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Usage: ./RPCservice_client [serverhost] [url/file]" << endl;
    return -1;
  }

  // argv[1] contains the server hostname
  boost::shared_ptr<TTransport> socket(new TSocket(argv[1], 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  RPCserviceClient client(protocol);

  //Open file
  ifstream inputFile(argv[2]);
  //Open file for writing response
  ofstream outputFile("out.txt", std::fstream::trunc);
  string line;
  if (inputFile.is_open() && outputFile.is_open()){ 
      try {
        transport->open();
        while ( getline (inputFile,line) ){
          responseData answer;
          client.getWebPage(answer, line);
          if (answer.isSuccess == 0){
            cout << "Request Failed for " << line << endl; 
          }
          else {
            outputFile << " ----- Web Page Content that you requested for ";
            outputFile << line;
            outputFile << "\n";
            outputFile << answer.webPage;
            outputFile << "\n\n";
          //  cout << " ----- Web Page Content that you requested is " << answer.webPage << endl;
          }
        }
        transport->close();
      } 
      catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
  }
  inputFile.close();
outputFile.close();

}
else{
  cout << "Error in opening input/output files\n";
}

  return 0;
}