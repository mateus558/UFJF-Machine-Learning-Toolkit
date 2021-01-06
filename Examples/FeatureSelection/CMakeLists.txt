add_executable(examp_golub examp_golub.cpp)
set_target_properties(examp_golub PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_golub ${LIBFEATSELECT})

add_executable(examp_fisher examp_fisher.cpp)
set_target_properties(examp_fisher PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_fisher ${LIBFEATSELECT})

add_executable(examp_rfe examp_rfe.cpp)
set_target_properties(examp_rfe PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_rfe ${LIBFEATSELECT})
