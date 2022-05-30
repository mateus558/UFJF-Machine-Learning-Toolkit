configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/featselect/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/iris.data COPYONLY)

add_executable(${FEATSELECT_TESTS} main.cpp featselect_tests.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${FEATSELECT_TESTS} gtest gtest_main core classifier valid visual featselect)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${FEATSELECT_TESTS})
#gtest_discover_tests(data_test)
add_test(NAME ${FEATSELECT_TESTS} COMMAND ${FEATSELECT_TESTS})