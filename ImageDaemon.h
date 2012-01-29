/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ImageDaemon_H
#define ImageDaemon_H

#include <TProcessor.h>
#include "ImageDaemon_types.h"

namespace ImageDaemon {

class ImageDaemonIf {
 public:
  virtual ~ImageDaemonIf() {}
  virtual void getBoWFeature(std::vector<Bin> & _return, const int64_t rowKey) = 0;
  virtual void addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray) = 0;
  virtual void addImage(const std::string& imageHash, const int64_t imageKey) = 0;
  virtual void computeColorFeature(const int64_t rowKey) = 0;
  virtual void query(std::vector<std::string> & _return, const std::string& imagePath) = 0;
};

class ImageDaemonIfFactory {
 public:
  typedef ImageDaemonIf Handler;

  virtual ~ImageDaemonIfFactory() {}

  virtual ImageDaemonIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ImageDaemonIf* /* handler */) = 0;
};

class ImageDaemonIfSingletonFactory : virtual public ImageDaemonIfFactory {
 public:
  ImageDaemonIfSingletonFactory(const boost::shared_ptr<ImageDaemonIf>& iface) : iface_(iface) {}
  virtual ~ImageDaemonIfSingletonFactory() {}

  virtual ImageDaemonIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ImageDaemonIf* /* handler */) {}

 protected:
  boost::shared_ptr<ImageDaemonIf> iface_;
};

class ImageDaemonNull : virtual public ImageDaemonIf {
 public:
  virtual ~ImageDaemonNull() {}
  void getBoWFeature(std::vector<Bin> & /* _return */, const int64_t /* rowKey */) {
    return;
  }
  void addPostingList(const int64_t /* visualwordID */, const std::vector<Posting> & /* postingArray */) {
    return;
  }
  void addImage(const std::string& /* imageHash */, const int64_t /* imageKey */) {
    return;
  }
  void computeColorFeature(const int64_t /* rowKey */) {
    return;
  }
  void query(std::vector<std::string> & /* _return */, const std::string& /* imagePath */) {
    return;
  }
};

typedef struct _ImageDaemon_getBoWFeature_args__isset {
  _ImageDaemon_getBoWFeature_args__isset() : rowKey(false) {}
  bool rowKey;
} _ImageDaemon_getBoWFeature_args__isset;

class ImageDaemon_getBoWFeature_args {
 public:

  ImageDaemon_getBoWFeature_args() : rowKey(0) {
  }

  virtual ~ImageDaemon_getBoWFeature_args() throw() {}

  int64_t rowKey;

  _ImageDaemon_getBoWFeature_args__isset __isset;

  void __set_rowKey(const int64_t val) {
    rowKey = val;
  }

  bool operator == (const ImageDaemon_getBoWFeature_args & rhs) const
  {
    if (!(rowKey == rhs.rowKey))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_getBoWFeature_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_getBoWFeature_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_getBoWFeature_pargs {
 public:


  virtual ~ImageDaemon_getBoWFeature_pargs() throw() {}

  const int64_t* rowKey;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ImageDaemon_getBoWFeature_result__isset {
  _ImageDaemon_getBoWFeature_result__isset() : success(false) {}
  bool success;
} _ImageDaemon_getBoWFeature_result__isset;

class ImageDaemon_getBoWFeature_result {
 public:

  ImageDaemon_getBoWFeature_result() {
  }

  virtual ~ImageDaemon_getBoWFeature_result() throw() {}

  std::vector<Bin>  success;

  _ImageDaemon_getBoWFeature_result__isset __isset;

  void __set_success(const std::vector<Bin> & val) {
    success = val;
  }

  bool operator == (const ImageDaemon_getBoWFeature_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_getBoWFeature_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_getBoWFeature_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ImageDaemon_getBoWFeature_presult__isset {
  _ImageDaemon_getBoWFeature_presult__isset() : success(false) {}
  bool success;
} _ImageDaemon_getBoWFeature_presult__isset;

class ImageDaemon_getBoWFeature_presult {
 public:


  virtual ~ImageDaemon_getBoWFeature_presult() throw() {}

  std::vector<Bin> * success;

  _ImageDaemon_getBoWFeature_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ImageDaemon_addPostingList_args__isset {
  _ImageDaemon_addPostingList_args__isset() : visualwordID(false), postingArray(false) {}
  bool visualwordID;
  bool postingArray;
} _ImageDaemon_addPostingList_args__isset;

class ImageDaemon_addPostingList_args {
 public:

  ImageDaemon_addPostingList_args() : visualwordID(0) {
  }

  virtual ~ImageDaemon_addPostingList_args() throw() {}

  int64_t visualwordID;
  std::vector<Posting>  postingArray;

  _ImageDaemon_addPostingList_args__isset __isset;

  void __set_visualwordID(const int64_t val) {
    visualwordID = val;
  }

  void __set_postingArray(const std::vector<Posting> & val) {
    postingArray = val;
  }

  bool operator == (const ImageDaemon_addPostingList_args & rhs) const
  {
    if (!(visualwordID == rhs.visualwordID))
      return false;
    if (!(postingArray == rhs.postingArray))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_addPostingList_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_addPostingList_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addPostingList_pargs {
 public:


  virtual ~ImageDaemon_addPostingList_pargs() throw() {}

  const int64_t* visualwordID;
  const std::vector<Posting> * postingArray;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addPostingList_result {
 public:

  ImageDaemon_addPostingList_result() {
  }

  virtual ~ImageDaemon_addPostingList_result() throw() {}


  bool operator == (const ImageDaemon_addPostingList_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ImageDaemon_addPostingList_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_addPostingList_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addPostingList_presult {
 public:


  virtual ~ImageDaemon_addPostingList_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ImageDaemon_addImage_args__isset {
  _ImageDaemon_addImage_args__isset() : imageHash(false), imageKey(false) {}
  bool imageHash;
  bool imageKey;
} _ImageDaemon_addImage_args__isset;

class ImageDaemon_addImage_args {
 public:

  ImageDaemon_addImage_args() : imageHash(""), imageKey(0) {
  }

  virtual ~ImageDaemon_addImage_args() throw() {}

  std::string imageHash;
  int64_t imageKey;

  _ImageDaemon_addImage_args__isset __isset;

  void __set_imageHash(const std::string& val) {
    imageHash = val;
  }

  void __set_imageKey(const int64_t val) {
    imageKey = val;
  }

  bool operator == (const ImageDaemon_addImage_args & rhs) const
  {
    if (!(imageHash == rhs.imageHash))
      return false;
    if (!(imageKey == rhs.imageKey))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_addImage_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_addImage_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addImage_pargs {
 public:


  virtual ~ImageDaemon_addImage_pargs() throw() {}

  const std::string* imageHash;
  const int64_t* imageKey;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addImage_result {
 public:

  ImageDaemon_addImage_result() {
  }

  virtual ~ImageDaemon_addImage_result() throw() {}


  bool operator == (const ImageDaemon_addImage_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ImageDaemon_addImage_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_addImage_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_addImage_presult {
 public:


  virtual ~ImageDaemon_addImage_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ImageDaemon_computeColorFeature_args__isset {
  _ImageDaemon_computeColorFeature_args__isset() : rowKey(false) {}
  bool rowKey;
} _ImageDaemon_computeColorFeature_args__isset;

class ImageDaemon_computeColorFeature_args {
 public:

  ImageDaemon_computeColorFeature_args() : rowKey(0) {
  }

  virtual ~ImageDaemon_computeColorFeature_args() throw() {}

  int64_t rowKey;

  _ImageDaemon_computeColorFeature_args__isset __isset;

  void __set_rowKey(const int64_t val) {
    rowKey = val;
  }

  bool operator == (const ImageDaemon_computeColorFeature_args & rhs) const
  {
    if (!(rowKey == rhs.rowKey))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_computeColorFeature_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_computeColorFeature_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_computeColorFeature_pargs {
 public:


  virtual ~ImageDaemon_computeColorFeature_pargs() throw() {}

  const int64_t* rowKey;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_computeColorFeature_result {
 public:

  ImageDaemon_computeColorFeature_result() {
  }

  virtual ~ImageDaemon_computeColorFeature_result() throw() {}


  bool operator == (const ImageDaemon_computeColorFeature_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ImageDaemon_computeColorFeature_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_computeColorFeature_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_computeColorFeature_presult {
 public:


  virtual ~ImageDaemon_computeColorFeature_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ImageDaemon_query_args__isset {
  _ImageDaemon_query_args__isset() : imagePath(false) {}
  bool imagePath;
} _ImageDaemon_query_args__isset;

class ImageDaemon_query_args {
 public:

  ImageDaemon_query_args() : imagePath("") {
  }

  virtual ~ImageDaemon_query_args() throw() {}

  std::string imagePath;

  _ImageDaemon_query_args__isset __isset;

  void __set_imagePath(const std::string& val) {
    imagePath = val;
  }

  bool operator == (const ImageDaemon_query_args & rhs) const
  {
    if (!(imagePath == rhs.imagePath))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_query_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_query_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ImageDaemon_query_pargs {
 public:


  virtual ~ImageDaemon_query_pargs() throw() {}

  const std::string* imagePath;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ImageDaemon_query_result__isset {
  _ImageDaemon_query_result__isset() : success(false) {}
  bool success;
} _ImageDaemon_query_result__isset;

class ImageDaemon_query_result {
 public:

  ImageDaemon_query_result() {
  }

  virtual ~ImageDaemon_query_result() throw() {}

  std::vector<std::string>  success;

  _ImageDaemon_query_result__isset __isset;

  void __set_success(const std::vector<std::string> & val) {
    success = val;
  }

  bool operator == (const ImageDaemon_query_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ImageDaemon_query_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ImageDaemon_query_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ImageDaemon_query_presult__isset {
  _ImageDaemon_query_presult__isset() : success(false) {}
  bool success;
} _ImageDaemon_query_presult__isset;

class ImageDaemon_query_presult {
 public:


  virtual ~ImageDaemon_query_presult() throw() {}

  std::vector<std::string> * success;

  _ImageDaemon_query_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ImageDaemonClient : virtual public ImageDaemonIf {
 public:
  ImageDaemonClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  ImageDaemonClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getBoWFeature(std::vector<Bin> & _return, const int64_t rowKey);
  void send_getBoWFeature(const int64_t rowKey);
  void recv_getBoWFeature(std::vector<Bin> & _return);
  void addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray);
  void send_addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray);
  void recv_addPostingList();
  void addImage(const std::string& imageHash, const int64_t imageKey);
  void send_addImage(const std::string& imageHash, const int64_t imageKey);
  void recv_addImage();
  void computeColorFeature(const int64_t rowKey);
  void send_computeColorFeature(const int64_t rowKey);
  void recv_computeColorFeature();
  void query(std::vector<std::string> & _return, const std::string& imagePath);
  void send_query(const std::string& imagePath);
  void recv_query(std::vector<std::string> & _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ImageDaemonProcessor : public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<ImageDaemonIf> iface_;
  virtual bool process_fn(apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid, void* callContext);
 private:
  std::map<std::string, void (ImageDaemonProcessor::*)(int32_t, apache::thrift::protocol::TProtocol*, apache::thrift::protocol::TProtocol*, void*)> processMap_;
  void process_getBoWFeature(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_addPostingList(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_addImage(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_computeColorFeature(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_query(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ImageDaemonProcessor(boost::shared_ptr<ImageDaemonIf> iface) :
    iface_(iface) {
    processMap_["getBoWFeature"] = &ImageDaemonProcessor::process_getBoWFeature;
    processMap_["addPostingList"] = &ImageDaemonProcessor::process_addPostingList;
    processMap_["addImage"] = &ImageDaemonProcessor::process_addImage;
    processMap_["computeColorFeature"] = &ImageDaemonProcessor::process_computeColorFeature;
    processMap_["query"] = &ImageDaemonProcessor::process_query;
  }

  virtual bool process(boost::shared_ptr<apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr<apache::thrift::protocol::TProtocol> poprot, void* callContext);
  virtual ~ImageDaemonProcessor() {}
};

class ImageDaemonProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ImageDaemonProcessorFactory(const ::boost::shared_ptr< ImageDaemonIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ImageDaemonIfFactory > handlerFactory_;
};

class ImageDaemonMultiface : virtual public ImageDaemonIf {
 public:
  ImageDaemonMultiface(std::vector<boost::shared_ptr<ImageDaemonIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ImageDaemonMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ImageDaemonIf> > ifaces_;
  ImageDaemonMultiface() {}
  void add(boost::shared_ptr<ImageDaemonIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getBoWFeature(std::vector<Bin> & _return, const int64_t rowKey) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        ifaces_[i]->getBoWFeature(_return, rowKey);
        return;
      } else {
        ifaces_[i]->getBoWFeature(_return, rowKey);
      }
    }
  }

  void addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      ifaces_[i]->addPostingList(visualwordID, postingArray);
    }
  }

  void addImage(const std::string& imageHash, const int64_t imageKey) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      ifaces_[i]->addImage(imageHash, imageKey);
    }
  }

  void computeColorFeature(const int64_t rowKey) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      ifaces_[i]->computeColorFeature(rowKey);
    }
  }

  void query(std::vector<std::string> & _return, const std::string& imagePath) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        ifaces_[i]->query(_return, imagePath);
        return;
      } else {
        ifaces_[i]->query(_return, imagePath);
      }
    }
  }

};

} // namespace

#endif
