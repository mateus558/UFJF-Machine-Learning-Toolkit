add_executable(${REGRESSOR_TESTS} main.cpp regressor_tests.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${REGRESSOR_TESTS} gtest gtest_main core visual regressor)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${REGRESSOR_TESTS})
#gtest_discover_tests(data_test)
add_test(NAME ${REGRESSOR_TESTS} COMMAND ${REGRESSOR_TESTS})