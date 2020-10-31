set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_executable(teste_valid teste_valid.cpp)
set_target_properties(teste_valid PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_valid ${LIBCORE} ${LIBCLASSIFIER})

add_executable(teste_data teste_data.cpp)
set_target_properties(teste_data PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_data ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(teste_ova teste_ova.cpp)
set_target_properties(teste_ova PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_ova ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(teste_ovo teste_ovo.cpp)
set_target_properties(teste_ovo PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_ovo ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(teste_osampling teste_oversampling.cpp)
set_target_properties(teste_osampling PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(teste_osampling ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})