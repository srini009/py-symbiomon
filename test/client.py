# (C) 2018 The University of Chicago
# See COPYRIGHT in top-level directory.
import sys
sys.path.append('.')
sys.path.append('build/lib.linux-x86_64-2.7')
import os
from pymargo import MargoInstance
from pysymbiomon.client import *

mid = MargoInstance('na+sm')

server_addr = sys.argv[1]
provider_id = int(sys.argv[2])

client = SymbiomonClient(mid)
addr = mid.lookup(server_addr)
ph = client.create_provider_handle(addr, provider_id)

del ph
del addr

client.finalize()
mid.finalize()
