add_library(${LIBFEATSELECT} SHARED
    src/AOS.cpp
    src/Fisher.cpp
    src/Golub.cpp
    src/RFE.cpp
)

set_target_properties(${LIBFEATSELECT} PROPERTIES PUBLIC_HEADER "include/FeatureSelection.hpp;include/AOS.hpp;include/Fisher.hpp;include/Golub.hpp;include/RFE.hpp")

target_include_directories(${LIBFEATSELECT} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBFEATSELECT} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBFEATSELECT} PUBLIC LIBFEATSELECT_VERSION=1.0)
target_compile_features(${LIBFEATSELECT} PRIVATE cxx_std_14)
target_link_libraries(${LIBFEATSELECT} ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})

install_lib(${LIBFEATSELECT} FeatureSelection)