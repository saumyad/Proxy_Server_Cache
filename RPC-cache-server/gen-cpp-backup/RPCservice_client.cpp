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

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "RPCservice.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace RPC;


int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Usage: ./RPCservice_client serverhost url" << endl;
    return -1;
  }

  // argv[1] contains the server hostname
  boost::shared_ptr<TTransport> socket(new TSocket(argv[1], 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  RPCserviceClient client(protocol);

  try {
    transport->open();
    responseData answer;
    client.getTime(answer, argv[2]);
    if (answer.isSuccess == 0){
      cout << "Request Failed" << endl; 
    }
    else {
      cout << " ----- Web Page Content that you requested is " << answer.webPage << endl;
    }
    transport->close();
  } 
  catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
  return 0;
}