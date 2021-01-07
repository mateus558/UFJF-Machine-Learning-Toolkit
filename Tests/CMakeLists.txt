add_executable(data_test_mltk data_test.cpp)
add_test(data_test data_test_mltk)

target_link_libraries(data_test_mltk ${LIBCORE})

add_executable(balltree_test_mltk balltree_test.cpp)
add_test(balltree_test balltree_test_mltk)

target_link_libraries(balltree_test_mltk ${LIBCORE})
