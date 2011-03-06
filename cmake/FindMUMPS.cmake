#
# MUMPS
#
# set WITH_MUMPS to YES to enable MUMPS support
# set MUMPS_ROOT to point to the directory containing your MUMPS library
#

# You can specify your own version of the library instead of the one provided by
# Femhub by specifying the environment variables MY_MUMPS_LIB_DIRS and 
# MY_MUMPS_INC_DIRS.
IF ("$ENV{MY_MUMPS_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_MUMPS_INC_DIRS}" STREQUAL "")
  # When linking the library to stand-alone Hermes, you may also specify the 
  # variables directly in CMake.vars
  IF (NOT MY_MUMPS_LIB_DIRS OR NOT MY_MUMPS_INC_DIRS)
    # Alternatively, you may simply specify MUMPS_ROOT in CMake.vars. This is 
    # the traditional way used also in the spkg files from the hpfem/solvers
    # repository and in the Hermes spkg.
    SET(MY_MUMPS_LIB_DIRS ${MUMPS_ROOT}/lib)
    SET(MY_MUMPS_INC_DIRS ${MUMPS_ROOT}/include)
  ENDIF (NOT MY_MUMPS_LIB_DIRS OR NOT MY_MUMPS_INC_DIRS)  
ELSE ("$ENV{MY_MUMPS_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_MUMPS_INC_DIRS}" STREQUAL "")
  SET(MY_MUMPS_LIB_DIRS $ENV{MY_MUMPS_LIB_DIRS})
  SET(MY_MUMPS_INC_DIRS $ENV{MY_MUMPS_INC_DIRS})
ENDIF ("$ENV{MY_MUMPS_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_MUMPS_INC_DIRS}" STREQUAL "")

# CMake maybe looks into the following paths by itself, but specifying them 
# explicitly doesn't hurt either.
SET(MUMPS_INCLUDE_SEARCH_PATH
	/usr/include
	/usr/local/include/
)

SET(MUMPS_LIB_SEARCH_PATH
	/usr/lib64
	/usr/lib
	/usr/local/lib/
)

FIND_PATH(MUMPS_INCLUDE_DIR  mumps_c_types.h ${MY_MUMPS_INC_DIRS}  NO_DEFAULT_PATH)
FIND_PATH(MUMPS_INCLUDE_DIR  mumps_c_types.h ${MUMPS_INCLUDE_SEARCH_PATH})

IF(NOT WITH_MPI)
  FIND_LIBRARY(MUMPS_MPISEQ_LIBRARY   mpiseq_seq        ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)
  FIND_LIBRARY(MUMPS_COMMON_LIBRARY   mumps_common_seq  ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)
  FIND_LIBRARY(MUMPS_PORD_LIBRARY     pord_seq          ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)
  
  FIND_LIBRARY(MUMPS_MPISEQ_LIBRARY   mpiseq_seq        ${MUMPS_LIB_SEARCH_PATH})
  FIND_LIBRARY(MUMPS_COMMON_LIBRARY   mumps_common_seq  ${MUMPS_LIB_SEARCH_PATH})
  FIND_LIBRARY(MUMPS_PORD_LIBRARY     pord_seq          ${MUMPS_LIB_SEARCH_PATH})
  
  FIND_PATH(MUMPS_MPISEQ_INCLUDE_PATH  mpi.h    ${MY_MUMPS_INC_DIRS}  NO_DEFAULT_PATH)
  FIND_PATH(MUMPS_MPISEQ_INCLUDE_PATH  mpi.h    ${MUMPS_INCLUDE_SEARCH_PATH})
  
  SET(MUMPS_INCLUDE_DIR ${MUMPS_INCLUDE_DIR} ${MUMPS_MPISEQ_INCLUDE_PATH})
  
  FIND_LIBRARY(MUMPSD_SEQ_LIBRARY dmumps_seq  ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)
  FIND_LIBRARY(MUMPSD_SEQ_LIBRARY dmumps_seq  ${MUMPS_LIB_SEARCH_PATH})
  LIST(APPEND REQUIRED_LIBRARIES "MUMPSD_SEQ_LIBRARY")  
  LIST(APPEND REQUIRED_LIBRARIES "MUMPS_MPISEQ_LIBRARY")
ELSE(NOT WITH_MPI)
  FIND_LIBRARY(MUMPS_COMMON_LIBRARY   mumps_common  ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)
  FIND_LIBRARY(MUMPS_PORD_LIBRARY     pord          ${MY_MUMPS_LIB_DIRS}  NO_DEFAULT_PATH)

  FIND_LIBRARY(MUMPS_COMMON_LIBRARY   mumps_common  ${MUMPS_LIB_SEARCH_PATH})
  FIND_LIBRARY(MUMPS_PORD_LIBRARY     pord          ${MUMPS_LIB_SEARCH_PATH})

  FIND_LIBRARY(MUMPSD_LIBRARY dmumps ${MY_MUMPS_LIB_DIRS}   NO_DEFAULT_PATH)
  FIND_LIBRARY(MUMPSD_LIBRARY dmumps ${MUMPS_LIB_SEARCH_PATH})
  LIST(APPEND REQUIRED_REAL_LIBRARIES "MUMPSD_LIBRARY")
ENDIF(NOT WITH_MPI)

LIST(APPEND REQUIRED_LIBRARIES "MUMPS_COMMON_LIBRARY" "MUMPS_PORD_LIBRARY")

# Test if all the required libraries have been found. If they haven't, end with fatal error...
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  MUMPS
   "MUMPS library could not be found. If your module requires to use its support 
   in Hermes, please install it according to instructions at
   <http://hpfem.org/hermes/doc/src/installation/matrix_solvers/mumps.html>\n" 
   ${REQUIRED_LIBRARIES} MUMPS_INCLUDE_DIR
) 

