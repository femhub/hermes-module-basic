# Sets compiler settings for a library.
macro(GENERAL_COMPILE_SETTINGS  LIB)
    # Enable debugging symbols for all files all the time:
    # set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Stop on first error (sometimes gcc doesn't print all the possible
        # candidates when a function signature is wrong if this is enabled, so we
        # leave it disabled by default):
        # set(CMAKE_CXX_FLAGS "-Wfatal-errors ${CMAKE_CXX_FLAGS}")

        # All warnings are errors
        #set(CMAKE_CXX_FLAGS "-Werror ${CMAKE_CXX_FLAGS}")

        # Disable the "unused variable/parameters/functions" warnings (as those are
        # not really problematic, it doesn't cause any harm):
        set(CMAKE_CXX_FLAGS "-Wno-unused-parameter -Wno-unused-variable -Wno-unused-function ${CMAKE_CXX_FLAGS}")

        # Disable all switch warnings for now (until all code is fixed)
        set(CMAKE_CXX_FLAGS "-Wno-switch-enum -Wno-switch -Wno-switch-default ${CMAKE_CXX_FLAGS}")

        # Enable all warnings:
        set(CMAKE_CXX_FLAGS "-W -Wuninitialized -Wall ${CMAKE_CXX_FLAGS}")

        set(RELEASE_FLAGS "-DNDEBUG -O3")
        set(DEBUG_FLAGS   "-g")
    endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    
    # Enabling multiprocessor build on MSVC
    if(MSVC)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # Do we have to add this in 
                                                    # set_target_properties?
    endif(MSVC)


    if(DEBUG)
        set_target_properties(${LIB} PROPERTIES COMPILE_FLAGS "${DEBUG_FLAGS}")
    else(DEBUG)
        set_target_properties(${LIB} PROPERTIES COMPILE_FLAGS "${RELEASE_FLAGS}")    
    endif(DEBUG)    
    
endmacro(GENERAL_COMPILE_SETTINGS)

# Sets linker settings.
macro(LIBRARY_LINK_SETTINGS LIB)
  set(CMAKE_REQUIRED_LIBRARIES m)
  
  if(NOT MSVC)
      IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
          target_link_libraries(${LIB} "rt")
      ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      if(HAVE_TEUCHOS_BFD)
          # stacktrace code needs LIButils:
          target_link_libraries(${LIB} bfd iberty)
      endif(HAVE_TEUCHOS_BFD)
  endif(NOT MSVC)
  
  # Link with all third party libraries corresponding to selected configuration options.
  # The respective variables will be empty for unselected TPL's.
  # Only the shared (.so) libraries need to be present).
  target_link_libraries(  ${LIB} 
      ${HERMES2D_REAL_LIBRARY}
      ${HERMES_COMMON_LIBRARY}
      ${GLUT_LIBRARY} ${GLEW_LIBRARY}
      ${HDF5_LIBRARY}
      ${UMFPACK_LIBRARIES}
      ${TRILINOS_LIBRARIES}
      ${PETSC_LIBRARIES}
      ${MUMPS_LIBRARIES}
      ${SUPERLU_LIBRARY}
      ${LAPACK_LIBRARIES}
      ${PTHREAD_LIBRARY}
      ${CLAPACK_LIBRARY} ${BLAS_LIBRARY} ${F2C_LIBRARY}
      ${ADDITIONAL_LIBS}
      ${PYTHON_LIBRARY}
      ${EXODUSII_LIBRARIES}
  )
  
  set_property(TARGET ${LIB} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
endmacro(LIBRARY_LINK_SETTINGS)

# Makes Win32 path from Unix-style path which is used by CMAKE. Used when the path is provided to an OS command.
macro(MAKE_PATH PATH_OUT PATH_IN)
	if(WIN32)
		string(REPLACE "/" "\\" ${PATH_OUT} ${PATH_IN})
	else(WIN32)
		set(${PATH_OUT} ${PATH_IN})
	endif(WIN32)
endmacro(MAKE_PATH)

# Installs a library to directories relative to CMAKE_INSTALL_PREFIX.
macro(LIBRARY_INSTALL_SETTINGS  LIB)
    # Copy target to a place which is accessible by the system. This is crucial for successful debugging using MSVC.
	if(MSVC)
		MAKE_PATH(TARGET_DIR "${TARGET_ROOT}/LIB")
		get_target_property(SOURCE_DEBUG_FILE ${LIB} LOCATION_Debug)
		MAKE_PATH(SOURCE_DEBUG_FILE ${SOURCE_DEBUG_FILE})
		get_target_property(SOURCE_RELEASE_FILE ${LIB} LOCATION_Release)
		MAKE_PATH(SOURCE_RELEASE_FILE ${SOURCE_RELEASE_FILE})
		add_custom_command(TARGET ${LIB}
			POST_BUILD
			COMMAND if exist ${SOURCE_DEBUG_FILE} copy /Y ${SOURCE_DEBUG_FILE} ${TARGET_DIR}
			COMMAND if exist ${SOURCE_RELEASE_FILE} copy /Y ${SOURCE_RELEASE_FILE} ${TARGET_DIR})
		unset(TARGET_DIR)
		unset(SOURCE_FILE)
	endif(MSVC)
	
	install(TARGETS ${LIB}
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
    )
endmacro(LIBRARY_INSTALL_SETTINGS) 

# MSVC (Win) helper macros

# Makes Win32 path from Unix-style patch which is used by CMAKE. Used when a path is provided to an OS utility.
macro(MAKE_PATH PATH_OUT PATH_IN)
  if(WIN32)
    string(REPLACE "/" "\\" ${PATH_OUT} ${PATH_IN})
  else(WIN32)
    set(${PATH_OUT} ${PATH_IN})
  endif(WIN32)
endmacro(MAKE_PATH)

# This ensures that a .dll library is built for both debug and release configurations under MSVC.
macro(ADD_MSVC_BUILD_FLAGS LIB LIB_DEBUG LIB_RELEASE)
  if(MSVC)
      get_target_property(FLAGS ${LIB} COMPILE_FLAGS)
      
      set_target_properties(${LIB} PROPERTIES COMPILE_FLAGS "-DEXPORT_HERMES_DLL ${FLAGS}")
      set_target_properties(${LIB} PROPERTIES DEBUG_OUTPUT_NAME ${LIB_DEBUG})
      set_target_properties(${LIB} PROPERTIES RELEASE_OUTPUT_NAME ${LIB_RELEASE})
  endif(MSVC)
endmacro(ADD_MSVC_BUILD_FLAGS)

