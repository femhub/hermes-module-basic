project(test-module-basic)

 add_executable(${PROJECT_NAME} main.cpp ../utils/disc.cpp)
 target_link_libraries(${PROJECT_NAME} ${MODULE_LIB})

 if(DEBUG)
      set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${DEBUG_FLAGS}")
  else(DEBUG)
      set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${RELEASE_FLAGS}")
  endif(DEBUG)

 add_test(test-module-basic ${PROJECT_NAME} ./model.cfg)