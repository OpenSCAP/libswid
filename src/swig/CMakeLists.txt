if (BUILD_PYTHON_BINDINGS)
	include(${SWIG_USE_FILE})

	set_property(SOURCE ${CMAKE_SOURCE_DIR}/src/swig/swid_api.i PROPERTY CPLUSPLUS ON)
	set_property(SOURCE ${CMAKE_SOURCE_DIR}/src/swig/swid_api.i PROPERTY SWIG_FLAGS -py3 -modern -safecstrings -I${CMAKE_SOURCE_DIR}/src)

	swig_add_library(swid_api MODULE LANGUAGE python SOURCES ${CMAKE_SOURCE_DIR}/src/swig/swid_api.i)
	swig_link_libraries(swid_api ${PYTHON_LIBRARY} swid)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in    ${CMAKE_BINARY_DIR}/setup.py)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

	target_include_directories(${SWIG_MODULE_swid_api_REAL_NAME} PUBLIC src ${PYTHON_INCLUDE_PATH} )
endif()


