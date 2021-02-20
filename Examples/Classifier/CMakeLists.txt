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

add_executable(knn_res knn_res.cpp)
set_target_properties(knn_res PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(knn_res ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

add_executable(knn_rsm_res knn_rsm_res.cpp)
set_target_properties(knn_rsm_res PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(knn_rsm_res ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})
