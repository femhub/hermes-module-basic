#
# HERMES2D_REAL
#

# You can specify your own version of the library instead of the one provided by
# Femhub by specifying the environment variables MY_HERMES2D_REAL_LIB_DIRS and 
# MY_HERMES2D_REAL_INC_DIRS.
IF ("$ENV{MY_HERMES2D_REAL_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_HERMES2D_REAL_INC_DIRS}" STREQUAL "")
  # When linking the library to stand-alone Hermes, you may also specify the 
  # variables directly in CMake.vars
  IF (NOT MY_HERMES2D_REAL_LIB_DIRS OR NOT MY_HERMES2D_REAL_INC_DIRS)
    # Alternatively, you may simply specify HERMES2D_REAL_ROOT in CMake.vars. This is 
    # the traditional way used also in the spkg files from the hpfem/solvers
    # repository and in the Hermes spkg.
    SET(MY_HERMES2D_REAL_LIB_DIRS ${HERMES_ROOT}/lib) 
    SET(MY_HERMES2D_REAL_INC_DIRS ${HERMES_ROOT}/include/hermes/hermes2d/src)
    SET(MY_HERMES_COMMON_LIB_DIRS ${HERMES_ROOT}/lib)
    SET(MY_HERMES_COMMON_INC_DIRS ${HERMES_ROOT}/include/hermes/hermes_common)
  ENDIF (NOT MY_HERMES2D_REAL_LIB_DIRS OR NOT MY_HERMES2D_REAL_INC_DIRS)
ELSE ("$ENV{MY_HERMES2D_REAL_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_HERMES2D_REAL_INC_DIRS}" STREQUAL "")
  SET(MY_HERMES2D_REAL_LIB_DIRS $ENV{MY_HERMES2D_REAL_LIB_DIRS})
  SET(MY_HERMES2D_REAL_INC_DIRS $ENV{MY_HERMES2D_REAL_INC_DIRS}/hermes2d/src)
  SET(MY_HERMES_COMMON_LIB_DIRS $ENV{MY_HERMES2D_REAL_LIB_DIRS})
  SET(MY_HERMES_COMMON_INC_DIRS $ENV{MY_HERMES2D_REAL_INC_DIRS}/hermes_common)
ENDIF ("$ENV{MY_HERMES2D_REAL_LIB_DIRS}" STREQUAL "" OR "$ENV{MY_HERMES2D_REAL_INC_DIRS}" STREQUAL "")

FIND_PATH(HERMES2D_REAL_INCLUDE_DIR hermes2d.h ${MY_HERMES2D_REAL_INC_DIRS}  NO_DEFAULT_PATH)
FIND_PATH(HERMES2D_REAL_INCLUDE_DIR hermes2d.h 
  /usr/include 
  /usr/include/hermes/hermes2d/src 
  /usr/local/include/ 
  /usr/local/include/hermes/hermes2d/src 
  /opt/local/hermes/hermes2d/src
  /usr/local
)

FIND_PATH(HERMES_COMMON_INCLUDE_DIR common.h ${MY_HERMES_COMMON_INC_DIRS}  NO_DEFAULT_PATH)
FIND_PATH(HERMES_COMMON_INCLUDE_DIR common.h 
  /usr/include 
  /usr/include/hermes/hermes_common
  /usr/local/include/ 
  /usr/local/include/hermes/hermes_common
  /opt/local/hermes/hermes_common
  /usr/local
)


IF (DEBUG)
    FIND_LIBRARY( HERMES2D_REAL_LIBRARY
      NAMES libhermes2d-real-debug hermes2d-real-debug   
      PATHS ${MY_HERMES2D_REAL_LIB_DIRS}  
      NO_DEFAULT_PATH
    )
    FIND_LIBRARY( HERMES_COMMON_LIBRARY
      NAMES libhermes_common-real-debug hermes_common-real-debug   
      PATHS ${MY_HERMES_COMMON_LIB_DIRS}  
      NO_DEFAULT_PATH
    )

    FIND_LIBRARY( HERMES2D_REAL_LIBRARY
      NAMES libhermes2d-real-debug hermes2d-real-debug   
      PATHS /usr/lib 
            /usr/lib/hermes/hermes2d/ 
            /usr/local/lib 
            /usr/local/lib/hermes/hermes2d 
            /opt/local/hermes/hermes2d/src
    )
    FIND_LIBRARY( HERMES_COMMON_LIBRARY
      NAMES libhermes_common-real-debug hermes_common-real-debug   
      PATHS /usr/lib 
            /usr/lib/hermes/hermes2d/ 
            /usr/local/lib 
            /usr/local/lib/hermes/hermes2d 
            /opt/local/hermes/hermes2d/src
)
ENDIF (DEBUG)

FIND_LIBRARY( HERMES2D_REAL_LIBRARY
  NAMES libhermes2d-real hermes2d-real   
  PATHS ${MY_HERMES2D_REAL_LIB_DIRS}  
  NO_DEFAULT_PATH
)
FIND_LIBRARY( HERMES_COMMON_LIBRARY
  NAMES libhermes_common-real hermes_common-real   
  PATHS ${MY_HERMES_COMMON_LIB_DIRS}  
  NO_DEFAULT_PATH
)

FIND_LIBRARY( HERMES2D_REAL_LIBRARY
  NAMES libhermes2d-real hermes2d-real   
  PATHS /usr/lib 
        /usr/lib/hermes/hermes2d/ 
        /usr/local/lib 
        /usr/local/lib/hermes/hermes2d 
        /opt/local/hermes/hermes2d/src
)
FIND_LIBRARY( HERMES_COMMON_LIBRARY
  NAMES libhermes_common-real hermes_common-real
  PATHS /usr/lib 
        /usr/lib/hermes/hermes2d/ 
        /usr/local/lib 
        /usr/local/lib/hermes/hermes2d 
        /opt/local/hermes/hermes2d/src
)

set(HERMES_LIBRARIES ${HERMES2D_REAL_LIBRARY} ${HERMES_COMMON_LIBRARY})
set(HERMES_INCLUDE_DIRS ${HERMES2D_REAL_INCLUDE_DIR} ${HERMES_COMMON_INCLUDE_DIR})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  HERMES 
  "Real version of the Hermes2D library not found. Please install it according to
  instructions at <http://hpfem.org/hermes/doc/index.html>.\n"
  HERMES_LIBRARIES HERMES_INCLUDE_DIRS
)

if (WITH_PYTHON)
    # Use a system directory into which Python puts all the modules by default.
    execute_process(
        COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" 
        OUTPUT_VARIABLE DEFAULT_PYTHON_INSTALL_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    FIND_PATH(  HERMES_WRAPPER_PXD
        NAMES   hermes2d.pxd
        PATHS   ${MY_HERMES2D_REAL_LIB_DIRS}/python/hermes2d
                ${PYTHON_INSTALL_PATH}/hermes2d
                ${DEFAULT_PYTHON_INSTALL_PATH}/hermes2d
    )
        
    SET(HERMES_PYTHON_WRAPPERS_DIR  ${HERMES_WRAPPER_PXD}/..)
    message(${HERMES_PYTHON_WRAPPERS_DIR})
    
    
    #  "Python wrappers for Hermes2D not found. Either disable them by setting WITH_PYTHON to NO,
    #  "or please install Hermes2D with Python wrappers according to instructions at 
    #  "<http://hpfem.org/hermes/doc/index.html>.\n"
    
        
endif (WITH_PYTHON)
