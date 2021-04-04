add_executable(data_test_mltk data_test.cpp)
add_test(data_test data_test_mltk)

target_link_libraries(data_test_mltk ${LIBCORE})

add_executable(covertree_test_mltk covertree_test.cpp hsnw_test.cpp)
add_test(covertree_test covertree_test_mltk)

target_link_libraries(covertree_test_mltk ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(selectfeats_test_mltk selecfeats_test.cpp)
add_test(selectfeats_test_mltk selectfeats_test_mltk)

target_link_libraries(selectfeats_test_mltk ${LIBCORE})
set (CMAKE_CXX_STANDARD 17)
add_executable(perms perms.cpp)
add_test(perms perms)
target_link_libraries(perms ${LIBCORE} -ltbb)

add_executable(dsm_test_mltk dsm_test.cpp)
add_test(dsm_test_mltk dsm_test_mltk)

target_link_libraries(dsm_test_mltk ${LIBCORE} ${LIBENSEMBLE} ${LIBVALIDATION})


add_executable(hsnw_test_mltk hsnw_test.cpp)
add_test(hsnw_test_mltk hsnw_test_mltk)

target_link_libraries(hsnw_test_mltk ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})


