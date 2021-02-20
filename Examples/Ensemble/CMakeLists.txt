add_executable(examp_ensemble examp_ensemble.cpp)
set_target_properties(examp_ensemble PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ensemble ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_perccom examp_perccom.cpp)
set_target_properties(examp_perccom PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_perccom ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_bag examp_bagging.cpp)
set_target_properties(examp_bag PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_bag ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_ada examp_adaboost.cpp)
set_target_properties(examp_ada PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_ada ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(examp_awvoting examp_awvoting.cpp)
set_target_properties(examp_awvoting PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(examp_awvoting ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(res_knn_ensem res_knn_ensem.cpp)
set_target_properties(res_knn_ensem PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_ensem ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(res_knn_bagging res_knn_bagging.cpp)
set_target_properties(res_knn_bagging PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_bagging ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(res_knn_rsm res_knn_rsm.cpp)
set_target_properties(res_knn_rsm PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_rsm ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(res_knn_ens_bag_w res_knn_ens_bag_w.cpp)
set_target_properties(res_knn_ens_bag_w PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_ens_bag_w ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})

add_executable(res_knn_ens_ww res_knn_ens_bag_ww.cpp)
set_target_properties(res_knn_ens_ww PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_ens_ww ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})


add_executable(res_knn_ens_bag_w_rsm res_knn_ens_bag_w_rsm.cpp)
set_target_properties(res_knn_ens_bag_w_rsm PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(res_knn_ens_bag_w_rsm ${LIBCORE} ${LIBCLASSIFIER} ${LIBENSEMBLE} ${LIBVALIDATION})
