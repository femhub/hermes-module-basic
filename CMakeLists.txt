project(module-basic)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_MODULE_PATH}/cxx_flag_overrides.cmake)

set(WITH_CXX        YES)
set(WITH_PYTHON     YES)
set(WITH_UI         NO)

set(DEBUG           YES)
set(RELEASE         NO)

set(WITH_TESTS      YES)

# Allow to override the default values in CMake.vars:
include(CMake.vars OPTIONAL)

# General purpose CMake macros.
include(CMakeMacros.txt)

IF (WITH_PYTHON)
    ENSURE_PYTHON()
ENDIF (WITH_PYTHON)

find_package(HERMES REQUIRED)
include_directories(${HERMES_INCLUDE_DIRS})

include(${HERMES_CONFIG_FILE})

if(WITH_UMFPACK)
    include_directories(${UMFPACK_INCLUDE_DIR})
endif(WITH_UMFPACK)
	
if(WITH_PETSC)
    include_directories(${PETSC_INCLUDE_DIR})
endif(WITH_PETSC)

if(WITH_MUMPS)
    include_directories(${MUMPS_INCLUDE_DIR})
endif(WITH_MUMPS)

if(WITH_TRILINOS)
    include_directories(${TRILINOS_INCLUDE_DIR})
endif(WITH_TRILINOS)

if(WITH_SUPERLU)
    include_directories(${SUPERLU_INCLUDE_DIR})

    if(SUPERLU_MT)
        add_definitions(-DSLU_MT)
    endif(SUPERLU_MT)
endif(WITH_SUPERLU)

if (WITH_OPENMP)
    find_package(OpenMP)    #TODO: Find what variables are set and include them
                            #   in Hermes' config file
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
endif (WITH_OPENMP)

if(WITH_HDF5)
    include_directories(${HDF5_INCLUDE_DIR})
endif(WITH_HDF5)

if(WITH_EXODUSII)
    include_directories(${EXODUSII_INCLUDE_DIR})
endif(WITH_EXODUSII)

if(NOT WITH_GLUT)
    add_definitions(-DNOGLUT)
endif(NOT WITH_GLUT)

# Solvers (TODO: This could maybe go into hermes_common/solver/CMakeLists.txt).
if(MSVC)
	if(CLAPACK_INCLUDE_DIRS)
  	    include_directories(${CLAPACK_INCLUDE_DIRS})
	endif(CLAPACK_INCLUDE_DIRS)
else(MSVC)
  # All third party solvers require BLAS and LAPACK (both found in the package LAPACK).
  # FindLAPACK is Fortran-only so Fortran must be enabled as well.
  # TODO: Find if this is still neccessary (there is no find_package(LAPACK) any more
  #       as the library path is fetched from Hermes' config file).
  # if(WITH_UMFPACK OR WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
  #   enable_language(Fortran)     
  #   add_definitions(-DWITH_BLAS)
  # endif(WITH_UMFPACK OR WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
endif(MSVC)  

include(ConfigureRPATH)
ConfigureRPATH()

if (WITH_CXX)
    # We require Python for the mesh parser (it also needs NumPy)
    ENSURE_PYTHON()
    add_subdirectory(src)
endif (WITH_CXX)

if (WITH_PYTHON)
    find_package(Cython REQUIRED)
    add_subdirectory(python)
endif (WITH_PYTHON)

if (WITH_UI)
    # TODO: What is needed to build the UI?
    add_subdirectory(ui)
endif (WITH_UI)    

if (WITH_TESTS)
  enable_testing()
  add_custom_target(test COMMAND ctest)
endif (WITH_TESTS)
