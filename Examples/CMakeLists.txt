set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_executable(examp_valid examp_valid.cpp)
set_target_properties(examp_valid PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_valid ${LIBCORE} ${LIBCLASSIFIER})

add_executable(examp_data examp_data.cpp)
set_target_properties(examp_data PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_data ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_ova examp_ova.cpp)
set_target_properties(examp_ova PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ova ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_ovo examp_ovo.cpp)
set_target_properties(examp_ovo PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ovo ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_osampling examp_oversampling.cpp)
set_target_properties(examp_osampling PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_osampling ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_ova_samp examp_ova_oversamp.cpp)
set_target_properties(examp_ova_samp PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ova_samp ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_ovo_samp examp_ovo_oversamp.cpp)
set_target_properties(examp_ovo_samp PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ovo_samp ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(examp_exp examp_expression.cpp)
set_target_properties(examp_exp PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_exp ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(bench_exptmp bench_exptmp.cpp)
set_target_properties(bench_exptmp PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(bench_exptmp ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})