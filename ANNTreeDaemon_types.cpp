/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "ANNTreeDaemon_types.h"

namespace net { namespace walnutvision {

const char* Neighbor::ascii_fingerprint = "056BD45B5249CAA453D3C7B115F349DB";
const uint8_t Neighbor::binary_fingerprint[16] = {0x05,0x6B,0xD4,0x5B,0x52,0x49,0xCA,0xA4,0x53,0xD3,0xC7,0xB1,0x15,0xF3,0x49,0xDB};

uint32_t Neighbor::read(::apache::thrift::protocol::TProtocol* iprot) {

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
          xfer += iprot->readI64(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->distance);
          this->__isset.distance = true;
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

uint32_t Neighbor::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Neighbor");
  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->id);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("distance", ::apache::thrift::protocol::T_DOUBLE, 2);
  xfer += oprot->writeDouble(this->distance);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

}} // namespace
