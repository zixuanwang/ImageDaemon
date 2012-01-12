/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "ImageDaemon.h"

namespace ImageDaemon {

uint32_t ImageDaemon_getBoWFeatureFromHbase_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->rowKey);
          this->__isset.rowKey = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ImageDaemon_getBoWFeatureFromHbase_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ImageDaemon_getBoWFeatureFromHbase_args");
  xfer += oprot->writeFieldBegin("rowKey", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->rowKey);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_getBoWFeatureFromHbase_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ImageDaemon_getBoWFeatureFromHbase_pargs");
  xfer += oprot->writeFieldBegin("rowKey", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64((*(this->rowKey)));
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_getBoWFeatureFromHbase_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->success.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            iprot->readListBegin(_etype3, _size0);
            this->success.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->success[_i4].read(iprot);
            }
            iprot->readListEnd();
          }
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ImageDaemon_getBoWFeatureFromHbase_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("ImageDaemon_getBoWFeatureFromHbase_result");

  if (this->__isset.success) {
    xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_LIST, 0);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->success.size()));
      std::vector<Bin> ::const_iterator _iter5;
      for (_iter5 = this->success.begin(); _iter5 != this->success.end(); ++_iter5)
      {
        xfer += (*_iter5).write(oprot);
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_getBoWFeatureFromHbase_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            (*(this->success)).clear();
            uint32_t _size6;
            ::apache::thrift::protocol::TType _etype9;
            iprot->readListBegin(_etype9, _size6);
            (*(this->success)).resize(_size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              xfer += (*(this->success))[_i10].read(iprot);
            }
            iprot->readListEnd();
          }
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ImageDaemon_addPostingList_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->visualwordID);
          this->__isset.visualwordID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->postingArray.clear();
            uint32_t _size11;
            ::apache::thrift::protocol::TType _etype14;
            iprot->readListBegin(_etype14, _size11);
            this->postingArray.resize(_size11);
            uint32_t _i15;
            for (_i15 = 0; _i15 < _size11; ++_i15)
            {
              xfer += this->postingArray[_i15].read(iprot);
            }
            iprot->readListEnd();
          }
          this->__isset.postingArray = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ImageDaemon_addPostingList_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ImageDaemon_addPostingList_args");
  xfer += oprot->writeFieldBegin("visualwordID", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->visualwordID);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("postingArray", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->postingArray.size()));
    std::vector<Posting> ::const_iterator _iter16;
    for (_iter16 = this->postingArray.begin(); _iter16 != this->postingArray.end(); ++_iter16)
    {
      xfer += (*_iter16).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_addPostingList_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ImageDaemon_addPostingList_pargs");
  xfer += oprot->writeFieldBegin("visualwordID", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64((*(this->visualwordID)));
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("postingArray", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>((*(this->postingArray)).size()));
    std::vector<Posting> ::const_iterator _iter17;
    for (_iter17 = (*(this->postingArray)).begin(); _iter17 != (*(this->postingArray)).end(); ++_iter17)
    {
      xfer += (*_iter17).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_addPostingList_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ImageDaemon_addPostingList_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("ImageDaemon_addPostingList_result");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t ImageDaemon_addPostingList_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

void ImageDaemonClient::getBoWFeatureFromHbase(std::vector<Bin> & _return, const int64_t rowKey)
{
  send_getBoWFeatureFromHbase(rowKey);
  recv_getBoWFeatureFromHbase(_return);
}

void ImageDaemonClient::send_getBoWFeatureFromHbase(const int64_t rowKey)
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("getBoWFeatureFromHbase", ::apache::thrift::protocol::T_CALL, cseqid);

  ImageDaemon_getBoWFeatureFromHbase_pargs args;
  args.rowKey = &rowKey;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();
}

void ImageDaemonClient::recv_getBoWFeatureFromHbase(std::vector<Bin> & _return)
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  if (fname.compare("getBoWFeatureFromHbase") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  ImageDaemon_getBoWFeatureFromHbase_presult result;
  result.success = &_return;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  if (result.__isset.success) {
    // _return pointer has now been filled
    return;
  }
  throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::MISSING_RESULT, "getBoWFeatureFromHbase failed: unknown result");
}

void ImageDaemonClient::addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray)
{
  send_addPostingList(visualwordID, postingArray);
  recv_addPostingList();
}

void ImageDaemonClient::send_addPostingList(const int64_t visualwordID, const std::vector<Posting> & postingArray)
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("addPostingList", ::apache::thrift::protocol::T_CALL, cseqid);

  ImageDaemon_addPostingList_pargs args;
  args.visualwordID = &visualwordID;
  args.postingArray = &postingArray;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();
}

void ImageDaemonClient::recv_addPostingList()
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  if (fname.compare("addPostingList") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  ImageDaemon_addPostingList_presult result;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  return;
}

bool ImageDaemonProcessor::process(boost::shared_ptr<apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr<apache::thrift::protocol::TProtocol> poprot, void* callContext) {

  ::apache::thrift::protocol::TProtocol* iprot = piprot.get();
  ::apache::thrift::protocol::TProtocol* oprot = poprot.get();
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;
  int32_t seqid;

  iprot->readMessageBegin(fname, mtype, seqid);

  if (mtype != ::apache::thrift::protocol::T_CALL && mtype != ::apache::thrift::protocol::T_ONEWAY) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::INVALID_MESSAGE_TYPE);
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return true;
  }

  return process_fn(iprot, oprot, fname, seqid, callContext);
}

bool ImageDaemonProcessor::process_fn(apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid, void* callContext) {
  std::map<std::string, void (ImageDaemonProcessor::*)(int32_t, apache::thrift::protocol::TProtocol*, apache::thrift::protocol::TProtocol*, void*)>::iterator pfn;
  pfn = processMap_.find(fname);
  if (pfn == processMap_.end()) {
    iprot->skip(apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD, "Invalid method name: '"+fname+"'");
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return true;
  }
  (this->*(pfn->second))(seqid, iprot, oprot, callContext);
  return true;
}

void ImageDaemonProcessor::process_getBoWFeatureFromHbase(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext)
{
  void* ctx = NULL;
  if (this->eventHandler_.get() != NULL) {
    ctx = this->eventHandler_->getContext("ImageDaemon.getBoWFeatureFromHbase", callContext);
  }
  apache::thrift::TProcessorContextFreer freer(this->eventHandler_.get(), ctx, "ImageDaemon.getBoWFeatureFromHbase");

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preRead(ctx, "ImageDaemon.getBoWFeatureFromHbase");
  }

  ImageDaemon_getBoWFeatureFromHbase_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  uint32_t bytes = iprot->getTransport()->readEnd();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postRead(ctx, "ImageDaemon.getBoWFeatureFromHbase", bytes);
  }

  ImageDaemon_getBoWFeatureFromHbase_result result;
  try {
    iface_->getBoWFeatureFromHbase(result.success, args.rowKey);
    result.__isset.success = true;
  } catch (const std::exception& e) {
    if (this->eventHandler_.get() != NULL) {
      this->eventHandler_->handlerError(ctx, "ImageDaemon.getBoWFeatureFromHbase");
    }

    apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("getBoWFeatureFromHbase", apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return;
  }

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preWrite(ctx, "ImageDaemon.getBoWFeatureFromHbase");
  }

  oprot->writeMessageBegin("getBoWFeatureFromHbase", apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postWrite(ctx, "ImageDaemon.getBoWFeatureFromHbase", bytes);
  }
}

void ImageDaemonProcessor::process_addPostingList(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext)
{
  void* ctx = NULL;
  if (this->eventHandler_.get() != NULL) {
    ctx = this->eventHandler_->getContext("ImageDaemon.addPostingList", callContext);
  }
  apache::thrift::TProcessorContextFreer freer(this->eventHandler_.get(), ctx, "ImageDaemon.addPostingList");

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preRead(ctx, "ImageDaemon.addPostingList");
  }

  ImageDaemon_addPostingList_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  uint32_t bytes = iprot->getTransport()->readEnd();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postRead(ctx, "ImageDaemon.addPostingList", bytes);
  }

  ImageDaemon_addPostingList_result result;
  try {
    iface_->addPostingList(args.visualwordID, args.postingArray);
  } catch (const std::exception& e) {
    if (this->eventHandler_.get() != NULL) {
      this->eventHandler_->handlerError(ctx, "ImageDaemon.addPostingList");
    }

    apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("addPostingList", apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return;
  }

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preWrite(ctx, "ImageDaemon.addPostingList");
  }

  oprot->writeMessageBegin("addPostingList", apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postWrite(ctx, "ImageDaemon.addPostingList", bytes);
  }
}

::boost::shared_ptr< ::apache::thrift::TProcessor > ImageDaemonProcessorFactory::getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) {
  ::apache::thrift::ReleaseHandler< ImageDaemonIfFactory > cleanup(handlerFactory_);
  ::boost::shared_ptr< ImageDaemonIf > handler(handlerFactory_->getHandler(connInfo), cleanup);
  ::boost::shared_ptr< ::apache::thrift::TProcessor > processor(new ImageDaemonProcessor(handler));
  return processor;
}
} // namespace

