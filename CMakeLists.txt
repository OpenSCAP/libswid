cmake_minimum_required(VERSION 3.0) # setting this is required
project(libswid)

###############################################################################
## file globbing ##############################################################
###############################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

file(GLOB_RECURSE sources      src/*.cpp src/*.h)
file(GLOB_RECURSE sources_test test/unit/*.cpp)
# You can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically.

find_package(XercesC)
find_package(TinyXML)
find_package(SWIG)

set(Python_ADDITIONAL_VERSIONS 3.7 3.6 3.5 3.4)
find_package(PythonLibs)

###############################################################################
## target definitions #########################################################
###############################################################################

# The data is just added to the executable, because in some IDEs (QtCreator) 
# files are invisible when they are not explicitly part of the project.
# add_executable(swid-util src/main.cpp)

# Just for example add some compiler flags.
# target_compile_options(swid-util PUBLIC -std=c++11 -Wall)

# This allows to include files relative to the root of the src directory with a <> pair
# target_include_directories(swid-util PUBLIC src)

add_library(swid SHARED ${sources})

# target_link_libraries(swid-util swid)

target_compile_options(swid PUBLIC -std=c++11 -Wall)

target_include_directories(swid PUBLIC src ${XercesC_INCLUDE_DIRS} ${TinyXML_INCLUDE_DIRS})
target_link_libraries(swid ${XercesC_LIBRARIES} ${TinyXML_LIBRARIES})

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)


include(${SWIG_USE_FILE})

set_property(SOURCE src/swig/swid.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE src/swig/swid.i PROPERTY SWIG_FLAGS -py3 -modern -safecstrings -I${CMAKE_CURRENT_SOURCE_DIR}/src)

swig_add_library(swid MODULE LANGUAGE python SOURCES src/swig/swid.i)
swig_link_libraries(swid ${PYTHON_LIBRARY} swid)

target_include_directories(${SWIG_MODULE_swid_REAL_NAME} PUBLIC src ${PYTHON_INCLUDE_PATH} )



###############################################################################
## testing ####################################################################
###############################################################################

add_executable(unit_tests ${sources_test} ${sources})

# Don't include libswid as a library, use sources.
# Not having explicit sources would destroy the coverage analysis.
#
# add_executable(unit_tests ${sources_test})
# target_link_libraries(unit_tests PUBLIC swid)
target_link_libraries(unit_tests ${XercesC_LIBRARIES} ${TinyXML_LIBRARIES})

# This define is added to prevent collision with the main.
# It might be better solved by not adding the source with the main to the
# testing target.
target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

target_include_directories(unit_tests PUBLIC src)

add_executable(resaver ${sources} test/integration/resaver.cpp)
target_include_directories(resaver PUBLIC src)
target_link_libraries(resaver PUBLIC ${XercesC_LIBRARIES} ${TinyXML_LIBRARIES})

add_executable(example-c++ docs/examples/report_creator_entity.cpp)
target_link_libraries(example-c++ PUBLIC swid)

include (CTest)

set(CTEST_ENVIRONMENT "PYTHONPATH=.")

add_test (NAME "unit" COMMAND unit_tests)
add_test (NAME "resaving" COMMAND resaver)
add_test (NAME "python-bindings" COMMAND py.test-3 ${CMAKE_CURRENT_SOURCE_DIR}/test/unit/test.py)
add_test (NAME "doc-examples" COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/integration/test-examples.sh "tinyxml.xml" "./example-c++" "${CMAKE_CURRENT_SOURCE_DIR}/docs/examples/report_creator_entity.py")

set_tests_properties("python-bindings" PROPERTIES ENVIRONMENT PYTHONPATH=.)
set_tests_properties("doc-examples" PROPERTIES ENVIRONMENT PYTHONPATH=.)

find_package(codecov)
add_coverage(swid)
add_coverage(unit_tests)
list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/test/*'")
coverage_evaluate()


###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS swid
	LIBRARY DESTINATION lib)
install(FILES src/include/libswid DESTINATION include)
install(FILES src/lib.h src/SWIDStruct.h DESTINATION include/swid)

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "libSWID")
set(CPACK_PACKAGE_VERSION "0.0.1")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

SET(CPACK_PACKAGE_VENDOR "Red Hat, Inc.")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# This must be last
include(CPack)
