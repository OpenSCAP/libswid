cmake_minimum_required(VERSION 3.0) # setting this is required
project(libswid)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

###############################################################################
## file globbing ##############################################################
###############################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

list(APPEND private_sources
	${CMAKE_SOURCE_DIR}/src/loader-generic.cpp ${CMAKE_SOURCE_DIR}/src/loader-generic.h
	${CMAKE_SOURCE_DIR}/src/loader-generic-notemplate.cpp
	${CMAKE_SOURCE_DIR}/src/libswid.cpp
	${CMAKE_SOURCE_DIR}/src/libswid-base.cpp
	${CMAKE_SOURCE_DIR}/src/SWIDStruct.cpp ${CMAKE_SOURCE_DIR}/src/SWIDStruct.h
	${CMAKE_SOURCE_DIR}/src/Translator.cpp ${CMAKE_SOURCE_DIR}/src/Translator.h
)

# We can't include extension-less files here, because they puzzle the coverage support:
# -> no append of ${CMAKE_SOURCE_DIR}/src/libswid
list(APPEND public_sources
	${CMAKE_SOURCE_DIR}/src/libswid.h
	${CMAKE_SOURCE_DIR}/src/libswid-base.h
	${CMAKE_SOURCE_DIR}/src/swid-common.h
)

# You can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically.

find_package(XercesC)
find_package(TinyXML)

find_package(SWIG)
find_package(PythonLibs 3)

find_package(codecov)

include(CMakeDependentOption)

CMAKE_DEPENDENT_OPTION(BUILD_XERCES_BACKEND "Whether to build Xerces-C backend" ON ${XercesC_FOUND} OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TINYXML_BACKEND "Whether to build TinyXML backend" ON ${TinyXML_FOUND} OFF)
CMAKE_DEPENDENT_OPTION(BUILD_PYTHON_BINDINGS "Whether to build Python3 bindings" ON ${SWIG_FOUND} OFF)

if (BUILD_XERCES_BACKEND)
	add_definitions(-DHAVE_XERCES)
	add_definitions(-DSCHEMA_ABS_PATH="${CMAKE_SOURCE_DIR}/resources/schema/schema.xsd")
	list(APPEND private_sources
		${CMAKE_SOURCE_DIR}/src/loader-xerces.cpp ${CMAKE_SOURCE_DIR}/src/loader-xerces.h
	)
endif()

if (BUILD_TINYXML_BACKEND)
	add_definitions(-DHAVE_TINYXML)
	list(APPEND private_sources
		${CMAKE_SOURCE_DIR}/src/loader-tinyxml.cpp ${CMAKE_SOURCE_DIR}/src/loader-tinyxml.h
	)
endif()

message(STATUS "Xerces-c library found: ${XercesC_FOUND}")
message(STATUS "TinyXML library found: ${TinyXML_FOUND}")
message(STATUS "SWIG found: ${SWIG_FOUND}")
message(STATUS "Building Xerces-c XML backend: ${BUILD_XERCES_BACKEND}")
message(STATUS "Building TinyXML XML backend: ${BUILD_TINYXML_BACKEND}")
message(STATUS "Building Python 3 bindings: ${BUILD_PYTHON_BINDINGS}")

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


add_library(swid SHARED)
target_sources(swid PUBLIC ${public_sources} PRIVATE ${private_sources})

# target_link_libraries(swid-util swid)

set_property(TARGET swid PROPERTY CXX_STANDARD 11)
set_property(TARGET swid PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_options(swid PUBLIC -Wall)

target_include_directories(swid PUBLIC src ${XercesC_INCLUDE_DIRS} ${TinyXML_INCLUDE_DIRS})
target_link_libraries(swid ${XercesC_LIBRARIES} ${TinyXML_LIBRARIES})

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)

enable_testing()
add_subdirectory("test")

# Python bindings - put everything to a 'swid' package directory
add_subdirectory("src/swig"  "swid")

add_coverage(swid)
list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
list(APPEND LCOV_REMOVE_PATTERNS "'${CMAKE_SOURCE_DIR}/test/*'")
coverage_evaluate()

###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS swid
	LIBRARY DESTINATION lib)
install(FILES ${CMAKE_SOURCE_DIR}/src/include/libswid DESTINATION include)
install(FILES ${CMAKE_SOURCE_DIR}/src/libswid-base.h src/SWIDStruct.h DESTINATION include/swid)

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "libSWID")
set(CPACK_PACKAGE_VERSION "${VERSION_FULL}")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

SET(CPACK_PACKAGE_VENDOR "libswid upstream")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_SOURCE_IGNORE_FILES "^\\\\..*;sonar-project.properties;codecov.yml")

# This must be last
include(CPack)
