configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/visualization/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/visualization/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/iris_mult.csv COPYONLY)

add_executable(${VISUALIZATION_TESTS} main.cpp vis_test.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${VISUALIZATION_TESTS} gtest gtest_main ufjfmltk-core ufjfmltk-visual ufjfmltk-classifier)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${VISUALIZATION_TESTS})
#gtest_discover_tests(data_test)
add_test(NAME ${VISUALIZATION_TESTS} COMMAND ${VISUALIZATION_TESTS})