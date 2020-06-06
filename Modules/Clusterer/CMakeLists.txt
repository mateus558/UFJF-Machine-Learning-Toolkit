add_library(${LIBCLUSTERER} SHARED
    src/KMeans.cpp
)

set_target_properties(${LIBCLUSTERER} PROPERTIES PUBLIC_HEADER "include/Clusterer.hpp;include/KMeans.hpp")

target_include_directories(${LIBCLUSTERER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${LIBCLUSTERER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBCLUSTERER} PUBLIC LIBCLUSTERER_VERSION=1.0)
target_compile_features(${LIBCLUSTERER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLUSTERER} ${LIBCORE})

include(GNUInstallDirs)

install(TARGETS ${LIBCLUSTERER}
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${LIBCLUSTERER}
        )