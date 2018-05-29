###############################################################################
## testing ####################################################################
###############################################################################
list(APPEND doctests "./example-c++" "./example-c")

file(GLOB_RECURSE sources_test_cxx ${CMAKE_SOURCE_DIR}/test/unit/*.cpp)
file(GLOB_RECURSE sources_test_c ${CMAKE_SOURCE_DIR}/test/unit/*.c)

add_executable(unit_tests++ ${sources_test_cxx})
add_executable(unit_tests ${sources_test_c})
add_executable(resaver ${CMAKE_SOURCE_DIR}/test/integration/resaver.cpp)

# Don't include libswid as a library, use sources.
# Not having explicit sources would destroy the coverage analysis.
#
# add_executable(unit_tests ${sources_test})
# target_link_libraries(unit_tests PUBLIC swid)
target_link_libraries(unit_tests++ PRIVATE swid)
target_link_libraries(unit_tests PRIVATE swid)
target_link_libraries(resaver PRIVATE swid)

# This define is added to prevent collision with the main.
# It might be better solved by not adding the source with the main to the
# testing target.
target_compile_definitions(unit_tests++ PUBLIC UNIT_TESTS)
target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

target_include_directories(unit_tests++ PUBLIC src)
target_include_directories(unit_tests PUBLIC src)
target_include_directories(resaver PUBLIC src)

target_link_libraries(resaver PUBLIC swid)


add_executable(example-c++ ${CMAKE_SOURCE_DIR}/docs/examples/report_creator_entity.cpp)
target_link_libraries(example-c++ PUBLIC swid)

add_executable(example-c ${CMAKE_SOURCE_DIR}/docs/examples/report_creator_entity.c)
target_link_libraries(example-c PUBLIC swid)

include (CTest)

add_test (NAME "Unit-C++" COMMAND unit_tests++)
add_test (NAME "Unit-C" COMMAND unit_tests)

set(SWID_VALIDATOR CACHE FILEPATH "Path to the swid validator .jar")

if (BUILD_PYTHON_BINDINGS)
	add_test (NAME "python-bindings" COMMAND py.test-3 ${CMAKE_SOURCE_DIR}/test/unit/test.py)
	set_tests_properties("python-bindings" PROPERTIES ENVIRONMENT PYTHONPATH=${CMAKE_BINARY_DIR})
	list(APPEND doctests "${CMAKE_SOURCE_DIR}/docs/examples/report_creator_entity.py")
endif()

add_test (NAME "doc-examples" COMMAND ${CMAKE_SOURCE_DIR}/test/integration/test-examples.sh "tinyxml.xml" ${doctests})
set_tests_properties("doc-examples" PROPERTIES ENVIRONMENT PYTHONPATH=${CMAKE_BINARY_DIR})

if (SWID_VALIDATOR)
add_test (NAME "resaving" COMMAND
	${CMAKE_SOURCE_DIR}/test/integration/resave.sh
	${SWID_VALIDATOR}
	${CMAKE_SOURCE_DIR}/resources/samples/Vault/Enterprise_Vault_Server.swidtag
	${CMAKE_SOURCE_DIR}/resources/samples/Vault/Enterprise_Vault.swidtag
	${CMAKE_SOURCE_DIR}/resources/samples/Vault/Enterprise_Vault_Outlook_Add-in.swidtag
)
endif()


add_coverage(unit_tests)
add_coverage(unit_tests++)
