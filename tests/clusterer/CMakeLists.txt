configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/clusterer/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/clusterer/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/tests/clusterer/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/wine.data COPYONLY)


add_executable(${CLUSTERER_TESTS} main.cpp clusterer_tests.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${CLUSTERER_TESTS} gtest gtest_main ufjfmltk-core ufjfmltk-clusterer)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${CLUSTERER_TESTS})
add_test(NAME ${CLUSTERER_TESTS} COMMAND ${CLUSTERER_TESTS})