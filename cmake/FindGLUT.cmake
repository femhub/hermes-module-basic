#
# GLUT
#

if(MSVC)
	set(GLUT_LIBRARY_NAME freeglut)
else(MSVC)
	set(GLUT_LIBRARY_NAME glut)
endif(MSVC)

FIND_LIBRARY(GLUT_LIBRARY ${GLUT_LIBRARY_NAME} ${GLUT_ROOT}/lib /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  GLUT
    "GLUT library not found. If your module requires to use its support in Hermes,
    please download and install it according to instructions at
    <http://hpfem.org/hermes/doc/index.html>.\n"
    GLUT_LIBRARY
)
