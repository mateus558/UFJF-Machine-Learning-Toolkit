configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris_mult.csv COPYONLY)
enable_testing()

################
# Define a test
add_executable(data_test data_test.cpp)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

target_link_libraries(data_test gtest_main ufjfmltk-core)
##################################
# Just make the test runnable with
#   $ make test

include(GoogleTest)
gtest_discover_tests(data_test)