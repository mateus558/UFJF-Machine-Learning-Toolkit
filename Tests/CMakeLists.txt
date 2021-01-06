add_executable(data_test_mltk data_test.cpp)
add_test(data_mltk data_test_mltk)

target_link_libraries(data_test_mltk ${LIBCORE})
