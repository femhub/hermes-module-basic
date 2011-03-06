#
# GLEW
#

if(MSVC)
	set(GLEW_LIBRARY_NAME glew32)
else(MSVC)
	set(GLEW_LIBRARY_NAME GLEW)
endif(MSVC)

FIND_LIBRARY(GLEW_LIBRARY ${GLEW_LIBRARY_NAME} ${GLEW_ROOT}/lib /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  GLEW 
    "GLEW library not found. It is a prerequisite for the GLUT support in Hermes.
    If you need to use it in your module, please download and install it according 
    to instructions at <http://hpfem.org/hermes/doc/index.html>.\n"
    GLEW_LIBRARY
)
