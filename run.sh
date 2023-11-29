#!/bin/bash

TOPOLOGY=minindn/topologies/topo-anel-loop.conf
LOG=ndvr_build.log

set -o pipefail

# rm logs &&
# ln -sf /tmp/minindn/ logs &&
./waf configure --debug &&
./waf && 
sudo ./waf install &&
sudo mn -c && 
sudo python minindn/ndvr.py $TOPOLOGY 
# sudo rm -rf logs_copy &&
# sudo cp -r /tmp/minindn/ logs_copy &&
# sudo chmod 755 -R logs_copy 
