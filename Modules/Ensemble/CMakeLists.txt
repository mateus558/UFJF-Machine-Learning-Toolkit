add_library(${LIBENSEMBLE} SHARED
        Ensemble.hpp
        )

set_target_properties(${LIBENSEMBLE} PROPERTIES LINKER_LANGUAGE CXX)


set_target_properties(${LIBENSEMBLE} PROPERTIES PUBLIC_HEADER
        "Ensemble.hpp;include/Ensemble.hpp;include/VotingClassifier.hpp;include/BaggingClassifier.hpp
        include/AdaBoostClassifier.hpp;include/AutoWeightedVoting.hpp"
        )

target_include_directories(${LIBENSEMBLE} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_definitions(${LIBENSEMBLE} PUBLIC LIBENSEMBLE_VERSION=1.0)
target_compile_features(${LIBENSEMBLE} PRIVATE cxx_std_17)
target_link_libraries(${LIBENSEMBLE} ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

install_lib(${LIBENSEMBLE} Ensemble 1.0)