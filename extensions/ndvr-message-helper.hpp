#ifndef _NDVR_HELPER_HPP_
#define _NDVR_HELPER_HPP_

#include "ndvr-message.pb.h"
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

inline RoutingTable DecodeDvInfo(const proto::DvInfo &dvinfo_proto) {
  RoutingTable dvinfo;
  // d-site => [[a,b,c], ...]
  std::map<std::string, PathVectors> prefixPathVector;

  for (int i = 0; i < dvinfo_proto.entry_size(); ++i) {

    const auto &entry = dvinfo_proto.entry(i);
    auto prefix = entry.prefix();
    auto seq = entry.seq();
    auto originator = entry.originator();
    auto cost = entry.cost();
    // auto bestnexthop = entry.bestnexthop();
    // auto sec_cost = entry.sec_cost();
    std::cout << "###### DecodeDvInfo ######" << std::endl;
    std::cout << "  prefix = " << prefix << ", seq = " << seq
              << ", originator = " << originator << ", cost = " << cost
              << std::endl;

    // TODO add multiple nexthops
    std::vector<std::string> ids;

    for (int j = 0; j < entry.next_hops().router_id_size(); ++j) {
      ids.push_back(entry.next_hops().router_id(j));
    }

    std::cout << "  next hops = " << join(ids, ",") << std::endl;
    // std::cout << "### >> prefix     :" << routerPrefix_Uri << std::endl;

    NextHop nextHop = NextHop(ids);

    auto it = prefixPathVector.find(prefix);
    if (it == prefixPathVector.end()) {
      prefixPathVector[prefix] = PathVectors();
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

inline RoutingTable DecodeDvInfo(const void *buf, size_t buf_size) {
  proto::DvInfo dvinfo_proto;
  if (!dvinfo_proto.ParseFromArray(buf, buf_size)) {
    RoutingTable res;
    return res;
  }
  return DecodeDvInfo(dvinfo_proto);
}
} // namespace ndvr
} // namespace ndn

#endif // _NDVR_HELPER_HPP_
