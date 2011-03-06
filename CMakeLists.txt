project(module-basic)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_MODULE_PATH}/cxx_flag_overrides.cmake)

set(WITH_CXX        YES)
set(WITH_PYTHON     YES)
set(WITH_UI         NO)

set(WITH_UMFPACK    NO)
set(WITH_SUPERLU    NO)
set(SUPERLU_MT      NO)
set(WITH_OPENMP     NO)
set(WITH_PETSC      NO)
set(WITH_MUMPS      NO)
set(WITH_TRILINOS   NO)
set(WITH_EXODUSII   NO)
set(WITH_HDF5       NO)
set(WITH_GLUT       NO)

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

find_package(PTHREAD REQUIRED)

find_package(GLEW)
if(GLEW_FOUND)
    find_package(GLUT)
    
    if(GLUT_FOUND)
        set(WITH_GLUT   YES)
    endif(GLUT_FOUND)        
endif(GLEW_FOUND)

find_package(UMFPACK)
if(UMFPACK_FOUND)
    set(WITH_UMFPACK    YES)
    include_directories(${UMFPACK_INCLUDE_DIR})
endif(UMFPACK_FOUND)    
	
find_package(PETSC)
if(PETSC_FOUND)
    set(WITH_PETSC   YES)
    include_directories(${PETSC_INCLUDE_DIR})
endif(PETSC_FOUND)

find_package(MUMPS)
if(MUMPS_FOUND)
    set(WITH_MUMPS   YES)
    include_directories(${MUMPS_INCLUDE_DIR})
endif(MUMPS_FOUND)

find_package(TRILINOS)
if(TRILINOS_FOUND)
    set(WITH_TRILINOS   YES)
    include_directories(${TRILINOS_INCLUDE_DIR})
endif(TRILINOS_FOUND)

find_package(SUPERLU)
if(SUPERLU_FOUND)
    set(WITH_SUPERLU   YES)
    include_directories(${SUPERLU_INCLUDE_DIR})

    if(SUPERLU_MT)
        add_definitions(-DSLU_MT)
    endif(SUPERLU_MT)
endif(SUPERLU_FOUND)	  

if (WITH_OPENMP)
    find_package(OpenMP)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
endif (WITH_OPENMP)

find_package(HDF5)
if(HDF5_FOUND)
    set(WITH_HDF5   NO)
    include_directories(${HDF5_INCLUDE_DIR})
endif(HDF5_FOUND)

find_package(EXODUSII)
if(EXODUSII_FOUND)
    set(WITH_EXODUSII   YES)
    include_directories(${EXODUSII_INCLUDE_DIR})
endif(EXODUSII_FOUND)

# Solvers (TODO: This could maybe go into hermes_common/solver/CMakeLists.txt).
if(MSVC)
	if(WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
	    if (NOT CLAPACK_FOUND)
    		find_package(CLAPACK REQUIRED)
	    	include_directories(${CLAPACK_INCLUDE_DIRS})
	    endif (NOT CLAPACK_FOUND)
	 
	    # If the library was found, prevent searching for it again in subsequent 
	    # invocations of cmake .
	    set(CLAPACK_FOUND    YES CACHE STRING "Lapack/BLAS library.")   
	endif(WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
else(MSVC)
  # All third party solvers require BLAS and LAPACK (both found in the package LAPACK).
  # FindLAPACK is Fortran-only so Fortran must be enabled as well.
  if(WITH_UMFPACK OR WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
    if (NOT LAPACK_FOUND)
        enable_language(Fortran)
        find_package(LAPACK REQUIRED)
    endif (NOT LAPACK_FOUND)
    
    add_definitions(-DWITH_BLAS)
    
    # If the library was found, prevent searching for it again in subsequent 
	# invocations of cmake .
	set(LAPACK_FOUND    YES CACHE STRING "Lapack/BLAS library.")
  endif(WITH_UMFPACK OR WITH_PETSC OR WITH_MUMPS OR WITH_TRILINOS OR WITH_SUPERLU)
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
