set(EXECNAME crossvalid)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_executable(${EXECNAME} valid.cpp)
set_target_properties(${EXECNAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(${EXECNAME} ${LIBCORE} ${LIBCLASSIFIER})

add_executable(teste_ova teste_ova.cpp)
set_target_properties(teste_ova PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_ova ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(teste_ovo teste_ovo.cpp)
set_target_properties(teste_ovo PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_ovo ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})
