configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/ensemble/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/ensemble/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/tests/ensemble/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/wine.data COPYONLY)


add_executable(${ENSEMBLE_TESTS} main.cpp ensemble_tests.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${ENSEMBLE_TESTS} gtest gtest_main core classifier ensemble valid)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${ENSEMBLE_TESTS})
add_test(NAME ${ENSEMBLE_TESTS} COMMAND ${ENSEMBLE_TESTS})