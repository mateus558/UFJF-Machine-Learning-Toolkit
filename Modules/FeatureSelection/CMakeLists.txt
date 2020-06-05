add_library(${LIBFEATSELECT} SHARED
    src/FeatureSelection.hpp
    src/AOS.hpp
    src/AOS.cpp
    src/Fisher.hpp
    src/Fisher.cpp
    src/Golub.hpp
    src/Golub.cpp
    src/RFE.hpp
    src/RFE.cpp
)

target_include_directories(${LIBFEATSELECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBFEATSELECT} PUBLIC LIBFEATSELECT_VERSION=1.0)
target_compile_features(${LIBFEATSELECT} PRIVATE cxx_std_14)
target_link_libraries(${LIBFEATSELECT} ${LIBCORE} ${LIBCLASSIFIER} ${LIBVALIDATION})
