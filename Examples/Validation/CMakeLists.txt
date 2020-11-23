add_executable(examp_valid examp_valid.cpp)
set_target_properties(examp_valid PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_valid ${LIBCORE} ${LIBCLASSIFIER})