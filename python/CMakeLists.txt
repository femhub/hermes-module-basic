include_directories(${PROJECT_BINARY_DIR}/src)

set(CYTHON_INCLUDE_DIRECTORIES . -I${HERMES_PYTHON_WRAPPERS_DIR})
CYTHON_ADD_MODULE(basic)
target_link_libraries(basic hermes_basic)
LIBRARY_LINK_SETTINGS(basic)

set(PY_PATH ${PYTHON_INSTALL_PATH}/hermes_modules)
install(TARGETS basic
            RUNTIME DESTINATION ${PY_PATH}
            ARCHIVE DESTINATION ${PY_PATH}
            LIBRARY DESTINATION ${PY_PATH}
        )
install(FILES __init__.py basic_defs.pxd
    DESTINATION ${PY_PATH}
    )
