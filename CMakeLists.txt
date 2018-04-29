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

add_library(libswid SHARED ${sources})

# target_link_libraries(swid-util libswid)

target_compile_options(libswid PUBLIC -std=c++11 -Wall)

target_include_directories(libswid PUBLIC src ${XercesC_INCLUDE_DIRS} ${TinyXML_INCLUDE_DIRS})
target_link_libraries(libswid ${XercesC_LIBRARIES} ${TinyXML_LIBRARIES})

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)


###############################################################################
## testing ####################################################################
###############################################################################

add_executable(unit_tests ${sources_test})

# This define is added to prevent collision with the main.
# It might be better solved by not adding the source with the main to the
# testing target.
target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

target_include_directories(unit_tests PUBLIC src)

target_link_libraries(unit_tests PUBLIC libswid)

include (CTest)

add_test ("Unit" unit_tests)

find_package(codecov)
add_coverage(libswid)
add_coverage(unit_tests)
list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/test/*'")
coverage_evaluate()


###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS DESTINATION example_destination)

# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package.
install(DIRECTORY resources DESTINATION example_destination)

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "libSWID")
set(CPACK_PACKAGE_VERSION "0.0.1")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
