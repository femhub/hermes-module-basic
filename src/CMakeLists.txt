# Set the name of the module library.
set(MODULE_LIB  hermes_basic)

#project(${MODULE_LIB})

#cmake_minimum_required(VERSION 2.6)

include(BuildAndInstallScripts)
CHECK_STACKTRACE()

if (NOT DEBUG AND NOT RELEASE)
  message(FATAL_ERROR "No versions are specified. 
                       Re-run with DEBUG and/or RELEASE set to YES.")
endif (NOT DEBUG AND NOT RELEASE)

# Create file with preprocessor definitions exposing the build settings to the source code.
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
	${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

add_library(${MODULE_LIB} SHARED basic.cpp)
set_target_properties(${MODULE_LIB} PROPERTIES DEBUG_OUTPUT_NAME "")
set_target_properties(${MODULE_LIB} PROPERTIES RELEASE_OUTPUT_NAME ${MODULE_LIB})

GENERAL_COMPILE_SETTINGS(${MODULE_LIB})
ADD_MSVC_BUILD_FLAGS(${MODULE_LIB} ${MODULE_LIB} ${MODULE_LIB}-debug)
LIBRARY_LINK_SETTINGS(${MODULE_LIB})
LIBRARY_INSTALL_SETTINGS(${MODULE_LIB})

add_executable(${PROJECT_NAME} main.cpp disc.cpp)
target_link_libraries(${PROJECT_NAME} ${MODULE_LIB})

if(WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif(WITH_TESTS)
