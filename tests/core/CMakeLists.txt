configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/core/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/core/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/iris_mult.csv COPYONLY)

set(CORE_TESTS core_test)

add_executable(${CORE_TESTS} main.cpp data_test.cpp stats_test.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${CORE_TESTS} gtest gtest_main ufjfmltk-core)
if(CODE_COVERAGE)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE ${CORE_TESTS}
            DEPENDENCIES ${CORE_TESTS} ufjfmltk-core
            EXCLUDE "cmake*"
    )
endif(CODE_COVERAGE)
##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${CORE_TESTS})
#gtest_discover_tests(data_test)
add_test(NAME ${CORE_TESTS} COMMAND ${CORE_TESTS})