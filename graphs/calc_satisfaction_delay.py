#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import sem, t
import matplotlib
import json
import argparse

parser = argparse.ArgumentParser(description='Calc satisfaction delay')
parser.add_argument('--log', metavar='FILE', nargs='+', required=True, default=[],
                    help='filename containing the logs for NDVR')
parser.add_argument('--tofile', metavar='FILE', type=str, nargs='?',
                    help='filename to save data')

args = parser.parse_args()

dvinfo = {}
dvinfo_seen = {}
satisfaction_delay = []
print "processing ndvr log file"
for filename in args.log:
    f = open(filename)
    for line in f:
        if line.find("ndn-cxx.nfd.Forwarder:onOutgoingInterest(): [DEBUG] onOutgoingInterest out=(257,0) interest=/localhop/ndvr/dvinfo/") != -1:
            elements = line.split(' ')
            time = float(elements[0][1:-1])
            n = elements[-1].replace('interest=', '')
            if n not in dvinfo:
                dvinfo[n] = time
        elif line.find("ndn.Ndvr:OnDvInfoContent(): [DEBUG] Received content for DV-Info: /localhop/ndvr/dvinfo") != -1:
            elements = line.split(' ')
            time = float(elements[0][1:-1])
            n = elements[-1]
            if n not in dvinfo_seen:
                assert n in dvinfo
                delta = time - dvinfo[n]
                satisfaction_delay.append(delta)
                dvinfo_seen[n] = delta


###############################
# Calc the mean and interval using 95% confidence level
print "calculating the mean and interval"
confidence = 0.95
satisfaction_delay_mean = np.average(satisfaction_delay)
satisfaction_delay_intv = sem(satisfaction_delay) * t.ppf((1 + confidence) / 2, len(satisfaction_delay) - 1)

if args.tofile:
    data={}
    data['satisfaction-delay'] = [satisfaction_delay_mean, satisfaction_delay_intv]
    data['data'] = satisfaction_delay
    with open(args.tofile, "w") as f:
        f.write(json.dumps(data))
else:
    print 'satisfaction-delay', satisfaction_delay_mean, satisfaction_delay_intv
    print 'max-delay', np.max(satisfaction_delay)

