# (C) 2018 The University of Chicago
# See COPYRIGHT in top-level directory.
import sys
sys.path.append('.')
#sys.path.append('build/lib.linux-x86_64-2.7')
import os
from pymargo.core import Engine
from pysymbiomon.client import *

mid = Engine('na+sm')

server_addr = sys.argv[1]
provider_id = int(sys.argv[2])

client = SymbiomonClient(mid)
addr = mid.lookup(server_addr)
id_ = client.metric_get_id("srini", "testmetric2", ["tag1", "tag2", "tag3"])
print ("Id is ", id_)
mh = client.create_metric_handle(addr, id_, provider_id)

samples = mh.metric_fetch(5)
print (samples)

metrics = client.list_metrics(addr, provider_id, 5)
print (metrics)

del mh
del addr

client.finalize()
mid.finalize()
