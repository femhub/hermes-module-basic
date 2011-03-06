#
# HDF5
#

#FIND_PATH(HDF5_INCLUDE_DIR hdf5.h /usr/include/ /usr/local/include/hdf5 ${HDF5_ROOT})
FIND_PATH(HDF5_INCLUDE_DIR hdf5.h ${HDF5_ROOT}/include)

#FIND_LIBRARY(HDF5_LIBRARY hdf5 /usr/lib/ /usr/local/lib/hdf5 ${HDF5_ROOT}) 
FIND_LIBRARY(HDF5_LIBRARY hdf5 ${HDF5_ROOT}/lib) 
#FIND_FILE(HDF5_LIBRARY libhdf5.a /usr/lib /usr/local/lib/hfd5 ${HDF5_ROOT}) 


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  HDF5
    "HDF5 library not found. If your module requires to use its support in Hermes,
    please download and install it according to instructions at
    <http://hpfem.org/hermes/doc/index.html>.\n"
    HDF5_LIBRARY HDF5_INCLUDE_DIR
)

IF (NOT HDF5_LIBRARY)
    set(HDF5_LIBRARY        "")
    set(HDF5_INCLUDE_DIR    "")
ENDIF (NOT HDF5_LIBRARY)    
