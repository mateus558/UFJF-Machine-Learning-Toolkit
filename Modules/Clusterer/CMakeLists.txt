add_library(${LIBCLUSTERER} SHARED
    src/KMeans.cpp
)

set_target_properties(${LIBCLUSTERER} PROPERTIES PUBLIC_HEADER "include/Clusterer.hpp;include/KMeans.hpp")

target_include_directories(${LIBCLUSTERER} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBCLUSTERER} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBCLUSTERER} PUBLIC LIBCLUSTERER_VERSION=1.0)
target_compile_features(${LIBCLUSTERER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLUSTERER} ${LIBCORE})

include(GNUInstallDirs)

install(TARGETS ${LIBCLUSTERER}
        EXPORT ${PROJECT_NAME}_${LIBCLUSTERER}-targets
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        RUNTIME DESTINATION
            ${RUNTIME_DIR}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${LIBCLUSTERER}
        )

install(EXPORT ${PROJECT_NAME}_${LIBCLUSTERER}-targets DESTINATION ${CMAKE_INSTALL_DIR}/cmake/${LIBCLUSTERER})