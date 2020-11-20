add_library(${LIBENSEMBLE} SHARED
        Ensemble.hpp
        src/BaggingClassifier.hpp
        src/AdaBoostClassifier.hpp
        )

set_target_properties(${LIBENSEMBLE} PROPERTIES LINKER_LANGUAGE CXX)


set_target_properties(${LIBENSEMBLE} PROPERTIES PUBLIC_HEADER
        "Ensemble.hpp;include/Ensemble.hpp;include/VotingClassifier.hpp"
        )

target_include_directories(${LIBENSEMBLE} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBENSEMBLE} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBENSEMBLE} PUBLIC LIBENSEMBLE_VERSION=1.0)
target_compile_features(${LIBENSEMBLE} PRIVATE cxx_std_17)
target_link_libraries(${LIBENSEMBLE} ${LIBCORE} ${LIBCLASSIFIER})

install_lib(${LIBENSEMBLE} Ensemble 1.0)