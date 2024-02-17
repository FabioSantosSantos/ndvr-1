#!/bin/bash

./waf configure --debug
./waf
sudo ./waf install
