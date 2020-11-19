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

add_executable(examp_ensemble examp_ensemble.cpp)
set_target_properties(examp_ensemble PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ensemble ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_perccom examp_perccom.cpp)
set_target_properties(examp_perccom PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_perccom ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_bag examp_bagging.cpp)
set_target_properties(examp_bag PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_bag ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})


add_executable(bench_exptmp bench_exptmp.cpp)
set_target_properties(bench_exptmp PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(bench_exptmp PUBLIC ${LIBCORE} OpenMP::OpenMP_CXX ${LIBCLASSIFIER} ${LIBVALIDATION})
else()
    target_link_libraries(bench_exptmp ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})
endif()