#ifndef _NDVR_HELPER_HPP_
#define _NDVR_HELPER_HPP_

#include "ndvr-message-ibf.pb.h"
#include "routing-table.hpp"

#include <iostream>
#include <bitset>

namespace ndn {
namespace ndvr {

template <typename T> std::string join(const T &v, const std::string &delim) {
  std::ostringstream s;
  for (const auto &i : v) {
    if (&i != &v[0]) {
      s << delim;
    }
    s << i;
  }
  return s.str();
}



inline RoutingTable DecodeDvInfoIBF(const proto::DvInfoIBF &dvinfo_proto) {
  RoutingTable dvinfo;
  // d-site => [[a,b,c], ...]
  std::map<std::string, PathVectorsIBFBased> prefixPathVector;

  for (int i = 0; i < dvinfo_proto.entry_size(); ++i) {

    const auto &entry = dvinfo_proto.entry(i);
    auto prefix = entry.prefix();
    auto seq = entry.seq();
    auto originator = entry.originator();
    auto number = entry.next_hops().number();

    auto nextHop = NextHopIBFBased(number);

    auto cost = nextHop.getCount();


    std::cout << "###### DecodeDvInfo ######" << std::endl;
    std::cout << "  prefix = " << prefix << ", seq = " << seq
              << ", originator = " << originator << ", cost = " << cost
              << ", count = " << nextHop.getCount()
              << std::endl;


    std::cout << "  next hops (number) = " << number << std::endl;
    std::cout << "  next hops (bits) = " << std::bitset<64>(number) << std::endl;

    auto it = prefixPathVector.find(prefix);

    if (it == prefixPathVector.end()) {
      prefixPathVector[prefix] = PathVectorsIBFBased();
    }

    auto &pathVectors = prefixPathVector[prefix];
    pathVectors.addPath(0, nextHop); // faceID = 0 is incorrect, but we will fix
                                     // it later in the processingDvInfo code
    std::cout << "  pathvector = " << pathVectors << std::endl;

    RoutingEntry re = RoutingEntry(prefix, seq, originator, cost, pathVectors);

    dvinfo[prefix] = re;
  }
  return dvinfo;
}

inline RoutingTable DecodeDvInfoIBF(const void *buf, size_t buf_size) {
  proto::DvInfoIBF dvinfo_proto;
  if (!dvinfo_proto.ParseFromArray(buf, buf_size)) {
    RoutingTable res;
    return res;
  }
  return DecodeDvInfoIBF(dvinfo_proto);
}
} // namespace ndvr
} // namespace ndn

#endif // _NDVR_HELPER_HPP_
