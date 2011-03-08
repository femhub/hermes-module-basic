#
# PETSc
#
# When you configure and install PETSc, set PETSC_ROOT to some /root/dir/of/petsc/
# and PETSC_ARCH to petsc-arch-real (if you intend to solve real problems) and/or
# petsc-arch-complex (if you intend to solve complex problems). Then, in order to
# configure Hermes with PETSc, you need to say (in global CMake.vars):
#   set(WITH_PETSC YES)
#   set(PETSC_ROOT /root/dir/of/petsc/)
#   set(PETSC_ARCH petsc-arch)
#
# Example:
#   In PETSc source directory (this is automatically done by the
#   standalone-install script when using the prepackaged library from hpfem/solvers):
#     ./config/configure.py PETSC_ARCH=linux-cxx-real --with-clanguage=cxx
#     make PETSC_DIR=/opt/petsc/petsc-3.1-p7 PETSC_ARCH=linux-cxx-real all
#     ./config/configure.py PETSC_ARCH=linux-cxx-complex --with-clanguage=cxx --with-scalar-type=complex
#     make PETSC_DIR=/opt/petsc/petsc-3.1-p7 PETSC_ARCH=linux-mpicxx-complex all
#
#   In hermes/CMake.vars:
#     set(WITH_PETSC YES)
#     set(PETSC_ROOT /opt/petsc/petsc-3.1-p7)
#     set(PETSC_ARCH linux-cxx)
#

# You can specify your own version of the library instead of the one provided by
# Femhub by specifying the environment variables MY_PETSC_LIB_DIRS and
# MY_PETSC_INC_DIRS.
IF ("$ENV{MY_PETSC_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_PETSC_INC_DIRS}" STREQUAL "")
  # When linking the library to stand-alone Hermes, you may also specify the
  # variables directly in CMake.vars
  IF (NOT MY_PETSC_LIB_DIRS OR NOT MY_PETSC_INC_DIRS)
    # Alternatively, you may simply specify PETSC_ROOT in CMake.vars. This is
    # the traditional way used also in the spkg files from the hpfem/solvers
    # repository and in the Hermes spkg.
    SET(MY_PETSC_LIB_DIRS ${PETSC_ROOT}/lib)
    SET(MY_PETSC_INC_DIRS ${PETSC_ROOT}/include)
  ENDIF (NOT MY_PETSC_LIB_DIRS OR NOT MY_PETSC_INC_DIRS)
ELSE ("$ENV{MY_PETSC_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_PETSC_INC_DIRS}" STREQUAL "")
  SET(MY_PETSC_LIB_DIRS $ENV{MY_PETSC_LIB_DIRS})
  SET(MY_PETSC_INC_DIRS $ENV{MY_PETSC_INC_DIRS})
ENDIF ("$ENV{MY_PETSC_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_PETSC_INC_DIRS}" STREQUAL "")

# Try to find petsc.h in the root include directory.
FIND_PATH(COMMON_PETSC_INCLUDE_DIR petsc.h PATHS ${MY_PETSC_INC_DIRS} NO_DEFAULT_PATH)
FIND_PATH(COMMON_PETSC_INCLUDE_DIR petsc.h)

IF(NOT WITH_MPI AND EXISTS ${MY_PETSC_INC_DIRS}/mpiuni)
  IF(COMMON_PETSC_INCLUDE_DIR)
    # Add path for the sequential emulation of MPI.
    SET(COMMON_PETSC_INCLUDE_DIR ${COMMON_PETSC_INCLUDE_DIR} ${MY_PETSC_INC_DIRS}/mpiuni)
  ELSE(COMMON_PETSC_INCLUDE_DIR)
    # Set path for the sequential emulation of MPI.
    SET(COMMON_PETSC_INCLUDE_DIR ${MY_PETSC_INC_DIRS}/mpiuni)
  ENDIF(COMMON_PETSC_INCLUDE_DIR)
ENDIF(NOT WITH_MPI AND EXISTS ${MY_PETSC_INC_DIRS}/mpiuni)

SET(PETSC_ARCH ${PETSC_ARCH}-real)

SET(PETSC_DIR ${MY_PETSC_LIB_DIRS}/${PETSC_ARCH})
IF(EXISTS ${PETSC_DIR})
  FIND_LIBRARY(PETSC_LIBRARIES petsc ${PETSC_DIR}/lib NO_DEFAULT_PATH)
  FIND_LIBRARY(PETSC_LIBRARIES petsc)

  IF(COMMON_PETSC_INCLUDE_DIR)
    # Add arch-specific include directory.
    SET(PETSC_INCLUDE_DIR ${COMMON_PETSC_INCLUDE_DIR} ${PETSC_DIR}/include)
  ELSE(COMMON_PETSC_INCLUDE_DIR)
    # petsc.h has not been found in the root include directory, search in the arch-specific one.
    FIND_PATH(PETSC_INCLUDE_DIR petsc.h PATHS ${PETSC_DIR}/include)
  ENDIF(COMMON_PETSC_INCLUDE_DIR)

  # linux specific (?)
  SET(PETSC_LIBRARIES ${PETSC_LIBRARIES} dl)
ENDIF(EXISTS ${PETSC_DIR})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  PETSC
    "PETSc library could not be found. If your module requires to use its
    support in Hermes, please install it according to instructions at
    <http://hpfem.org/hermes/doc/src/installation/matrix_solvers/petsc.html>\n"
    PETSC_LIBRARIES PETSC_INCLUDE_DIR
)

IF (NOT PETSC_LIBRARIES)
    set(PETSC_LIBRARIES      "")
    set(PETSC_INCLUDE_DIR    "")
ENDIF (NOT PETSC_LIBRARIES)

