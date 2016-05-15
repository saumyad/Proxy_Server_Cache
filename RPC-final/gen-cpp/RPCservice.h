/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef RPCservice_H
#define RPCservice_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "RPCserver_types.h"

namespace RPC {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class RPCserviceIf {
 public:
  virtual ~RPCserviceIf() {}
  virtual void getWebPage(responseData& _return, const std::string& url, const int32_t size) = 0;
};

class RPCserviceIfFactory {
 public:
  typedef RPCserviceIf Handler;

  virtual ~RPCserviceIfFactory() {}

  virtual RPCserviceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(RPCserviceIf* /* handler */) = 0;
};

class RPCserviceIfSingletonFactory : virtual public RPCserviceIfFactory {
 public:
  RPCserviceIfSingletonFactory(const boost::shared_ptr<RPCserviceIf>& iface) : iface_(iface) {}
  virtual ~RPCserviceIfSingletonFactory() {}

  virtual RPCserviceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(RPCserviceIf* /* handler */) {}

 protected:
  boost::shared_ptr<RPCserviceIf> iface_;
};

class RPCserviceNull : virtual public RPCserviceIf {
 public:
  virtual ~RPCserviceNull() {}
  void getWebPage(responseData& /* _return */, const std::string& /* url */, const int32_t /* size */) {
    return;
  }
};

typedef struct _RPCservice_getWebPage_args__isset {
  _RPCservice_getWebPage_args__isset() : url(false), size(false) {}
  bool url :1;
  bool size :1;
} _RPCservice_getWebPage_args__isset;

class RPCservice_getWebPage_args {
 public:

  RPCservice_getWebPage_args(const RPCservice_getWebPage_args&);
  RPCservice_getWebPage_args& operator=(const RPCservice_getWebPage_args&);
  RPCservice_getWebPage_args() : url(), size(0) {
  }

  virtual ~RPCservice_getWebPage_args() throw();
  std::string url;
  int32_t size;

  _RPCservice_getWebPage_args__isset __isset;

  void __set_url(const std::string& val);

  void __set_size(const int32_t val);

  bool operator == (const RPCservice_getWebPage_args & rhs) const
  {
    if (!(url == rhs.url))
      return false;
    if (!(size == rhs.size))
      return false;
    return true;
  }
  bool operator != (const RPCservice_getWebPage_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RPCservice_getWebPage_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RPCservice_getWebPage_pargs {
 public:


  virtual ~RPCservice_getWebPage_pargs() throw();
  const std::string* url;
  const int32_t* size;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RPCservice_getWebPage_result__isset {
  _RPCservice_getWebPage_result__isset() : success(false) {}
  bool success :1;
} _RPCservice_getWebPage_result__isset;

class RPCservice_getWebPage_result {
 public:

  RPCservice_getWebPage_result(const RPCservice_getWebPage_result&);
  RPCservice_getWebPage_result& operator=(const RPCservice_getWebPage_result&);
  RPCservice_getWebPage_result() {
  }

  virtual ~RPCservice_getWebPage_result() throw();
  responseData success;

  _RPCservice_getWebPage_result__isset __isset;

  void __set_success(const responseData& val);

  bool operator == (const RPCservice_getWebPage_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RPCservice_getWebPage_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RPCservice_getWebPage_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RPCservice_getWebPage_presult__isset {
  _RPCservice_getWebPage_presult__isset() : success(false) {}
  bool success :1;
} _RPCservice_getWebPage_presult__isset;

class RPCservice_getWebPage_presult {
 public:


  virtual ~RPCservice_getWebPage_presult() throw();
  responseData* success;

  _RPCservice_getWebPage_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class RPCserviceClient : virtual public RPCserviceIf {
 public:
  RPCserviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RPCserviceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getWebPage(responseData& _return, const std::string& url, const int32_t size);
  void send_getWebPage(const std::string& url, const int32_t size);
  void recv_getWebPage(responseData& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class RPCserviceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<RPCserviceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (RPCserviceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getWebPage(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  RPCserviceProcessor(boost::shared_ptr<RPCserviceIf> iface) :
    iface_(iface) {
    processMap_["getWebPage"] = &RPCserviceProcessor::process_getWebPage;
  }

  virtual ~RPCserviceProcessor() {}
};

class RPCserviceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  RPCserviceProcessorFactory(const ::boost::shared_ptr< RPCserviceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< RPCserviceIfFactory > handlerFactory_;
};

class RPCserviceMultiface : virtual public RPCserviceIf {
 public:
  RPCserviceMultiface(std::vector<boost::shared_ptr<RPCserviceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~RPCserviceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<RPCserviceIf> > ifaces_;
  RPCserviceMultiface() {}
  void add(boost::shared_ptr<RPCserviceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getWebPage(responseData& _return, const std::string& url, const int32_t size) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getWebPage(_return, url, size);
    }
    ifaces_[i]->getWebPage(_return, url, size);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class RPCserviceConcurrentClient : virtual public RPCserviceIf {
 public:
  RPCserviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RPCserviceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getWebPage(responseData& _return, const std::string& url, const int32_t size);
  int32_t send_getWebPage(const std::string& url, const int32_t size);
  void recv_getWebPage(responseData& _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif