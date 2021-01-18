# (C) 2018 The University of Chicago
# See COPYRIGHT in top-level directory.
import sys
sys.path.append('.')
sys.path.append('build/lib.linux-x86_64-2.7')
import os
from pymargo.core import Engine
from pysymbiomon.client import *

mid = Engine('na+sm')

server_addr = sys.argv[1]
provider_id = int(sys.argv[2])

client = SymbiomonClient(mid)
addr = mid.lookup(server_addr)
id_ = client.metric_get_id("srini", "testmetric2", ["tag1", "tag2", "tag3"])

#Hardcore id here for testing purposes, and id_ above returns some garbage
#when translating back up from C++
id_ = 1628746916
mh = client.create_metric_handle(addr, id_, provider_id)

samples = mh.metric_fetch(3)

del mh
del addr

client.finalize()
mid.finalize()
