from distutils.core import setup
from distutils.extension import Extension
from distutils.sysconfig import get_config_vars
import pybind11
import pkgconfig
import os
import os.path
import sys

(opt,) = get_config_vars('OPT')
os.environ['OPT'] = " ".join(
		    flag for flag in opt.split() if flag != '-Wstrict-prototypes'
		)

# Find out the dependencies using pkgconfig 
# For client...
pk = pkgconfig.parse('symbiomon-client')
client_libraries    = pk['libraries']
client_library_dirs = pk['library_dirs']
client_include_dirs = pk['include_dirs']
client_include_dirs.append(".")
client_include_dirs.append(pybind11.get_include())
#Ideally, these lines should not be here for the client, but for now, its okay
pk = pkgconfig.parse('symbiomon-server')
client_libraries += pk['libraries']
client_library_dirs += pk['library_dirs']

pysymbiomon_client_module = Extension('_pysymbiomonclient', ["pysymbiomon/src/client.cpp"],
		           libraries=client_libraries,
                   library_dirs=client_library_dirs,
                   include_dirs=client_include_dirs,
                   extra_compile_args=['-std=c++11'],
                   depends=["pysymbiomon/src/client.cpp"])

setup(name='pysymbiomon',
      version='0.1.1',
      author='Srinivasan Ramesh',
      description="""Python binding for SYMBIOMON""",
      ext_modules=[ pysymbiomon_client_module ],
      packages=['pysymbiomon']
     )
