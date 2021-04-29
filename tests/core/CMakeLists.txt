configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/core/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/core/iris_mult.csv COPYONLY)

################
# Define a test
#
add_executable(data_test data_test.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(data_test gtest gtest_main ufjfmltk-core)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET data_test)
#gtest_discover_tests(data_test)
add_test(NAME data_test COMMAND data_test)

add_executable(stats_test stats_test.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(stats_test gtest gtest_main ufjfmltk-core)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET stats_test)
#gtest_discover_tests(data_test)
add_test(NAME stats_test COMMAND stats_test)