#ifndef _NDVR_HELPER_HPP_
#define _NDVR_HELPER_HPP_

#include "ndvr-message-ibf.pb.h"
#include "routing-table.hpp"

#include <iostream>
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
    auto cost = entry.next_hops().count();
    auto count =  entry.next_hops().count();

    std::cout << "###### DecodeDvInfo ######" << std::endl;
    std::cout << "  prefix = " << prefix << ", seq = " << seq
              << ", originator = " << originator << ", cost = " << cost
              << ", count = " << count
              << std::endl;

    // TODO add multiple nexthops
    std::vector<size_t> bits;

    for (int j = 0; j < entry.next_hops().bits_ibf().size(); ++j) {
      bits.push_back(entry.next_hops().bits_ibf(j));
    }

    std::cout << "  next hops (bits) = " << join(bits, ",") << std::endl;
    // std::cout << "### >> prefix     :" << routerPrefix_Uri << std::endl;

    auto nextHop = NextHopIBFBased(count, bits);

    auto it = prefixPathVector.find(prefix);

    if (it == prefixPathVector.end()) {
      prefixPathVector[prefix] = PathVectorsIBFBased();
    }

    auto &pathVectors = prefixPathVector[prefix];
    pathVectors.addPath(0, nextHop); // faceID = 0 is incorrect, but we will fix
                                     // it later in the processingDvInfo code
    std::cout << "  pathvector = " << pathVectors << std::endl;

    RoutingEntry re = RoutingEntry(prefix, seq, originator, nextHop.getCost(), pathVectors);

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
