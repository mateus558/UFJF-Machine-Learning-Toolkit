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
