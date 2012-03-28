/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ImageDaemon_TYPES_H
#define ImageDaemon_TYPES_H

#include <Thrift.h>
#include <TApplicationException.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace net { namespace walnutvision {

typedef struct _Bin__isset {
  _Bin__isset() : visualwordID(false), frequency(false) {}
  bool visualwordID;
  bool frequency;
} _Bin__isset;

class Bin {
 public:

  static const char* ascii_fingerprint; // = "056BD45B5249CAA453D3C7B115F349DB";
  static const uint8_t binary_fingerprint[16]; // = {0x05,0x6B,0xD4,0x5B,0x52,0x49,0xCA,0xA4,0x53,0xD3,0xC7,0xB1,0x15,0xF3,0x49,0xDB};

  Bin() : visualwordID(0), frequency(0) {
  }

  virtual ~Bin() throw() {}

  int64_t visualwordID;
  double frequency;

  _Bin__isset __isset;

  void __set_visualwordID(const int64_t val) {
    visualwordID = val;
  }

  void __set_frequency(const double val) {
    frequency = val;
  }

  bool operator == (const Bin & rhs) const
  {
    if (!(visualwordID == rhs.visualwordID))
      return false;
    if (!(frequency == rhs.frequency))
      return false;
    return true;
  }
  bool operator != (const Bin &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Bin & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

}} // namespace

#endif
