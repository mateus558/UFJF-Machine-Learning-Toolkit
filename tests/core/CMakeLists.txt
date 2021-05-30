configure_file(${DATA_PATH}/iris.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/tests/core/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/tests/core/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/iris.data ${CMAKE_BINARY_DIR}/iris.data COPYONLY)
configure_file(${DATA_PATH}/iris_mult.csv ${CMAKE_BINARY_DIR}/iris_mult.csv COPYONLY)
configure_file(${DATA_PATH}/diabetic.arff ${CMAKE_BINARY_DIR}/diabetic.arff COPYONLY)
configure_file(${DATA_PATH}/diabetic.arff ${CMAKE_BINARY_DIR}/tests/core/diabetic.arff COPYONLY)
configure_file(${DATA_PATH}/diabetic.arff ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/diabetic.arff COPYONLY)
configure_file(${DATA_PATH}/seismic-bumps.arff ${CMAKE_BINARY_DIR}/seismic-bumps.arff COPYONLY)
configure_file(${DATA_PATH}/seismic-bumps.arff ${CMAKE_BINARY_DIR}/tests/core/seismic-bumps.arff COPYONLY)
configure_file(${DATA_PATH}/seismic-bumps.arff ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/seismic-bumps.arff COPYONLY)
configure_file(${DATA_PATH}/pima.data ${CMAKE_BINARY_DIR}/pima.data COPYONLY)
configure_file(${DATA_PATH}/pima.data ${CMAKE_BINARY_DIR}/tests/core/pima.data COPYONLY)
configure_file(${DATA_PATH}/pima.data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pima.data COPYONLY)
configure_file(${DATA_PATH}/teste.txt ${CMAKE_BINARY_DIR}/teste.txt COPYONLY)
configure_file(${DATA_PATH}/teste.txt ${CMAKE_BINARY_DIR}/tests/core/teste.txt COPYONLY)
configure_file(${DATA_PATH}/teste.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/teste.txt COPYONLY)

add_executable(${CORE_TESTS} main.cpp data_test.cpp stats_test.cpp)
######################################
# Configure the test to use GoogleTest
#
target_link_libraries(${CORE_TESTS} gtest gtest_main core valid classifier)

##################################
# Just make the test runnable with
#   $ make test
include(GoogleTest)
gtest_add_tests(TARGET ${CORE_TESTS})
#gtest_discover_tests(data_test)
add_test(NAME ${CORE_TESTS} COMMAND ${CORE_TESTS})