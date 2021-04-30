configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/classifier/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/classifier/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/tests/classifier/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_BINARY_DIR}/wine.data COPYONLY)
configure_file(${DATA_PATH}/wine.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/wine.data COPYONLY)


add_executable(${CLASSIFIER_TESTS} main.cpp class_tests.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${CLASSIFIER_TESTS} gtest gtest_main ufjfmltk-core ufjfmltk-classifier ufjfmltk-valid)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${CLASSIFIER_TESTS})
add_test(NAME ${CLASSIFIER_TESTS} COMMAND ${CLASSIFIER_TESTS})