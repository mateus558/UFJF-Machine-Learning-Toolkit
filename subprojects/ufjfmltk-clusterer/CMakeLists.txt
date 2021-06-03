set(CLUSTERER_TARGET clusterer)

# List of HPP (header) library files.
set(${CLUSTERER_TARGET}_HDR
        include/ufjfmltk/clusterer/Clusterer.hpp
        include/ufjfmltk/clusterer/KMeans.hpp
        )

# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${CLUSTERER_TARGET} INTERFACE)
add_library(${PROJECT_NAME}::${CLUSTERER_TARGET} ALIAS ${CLUSTERER_TARGET})

# Specify include directories for both compilation and installation process.
target_include_directories(${CLUSTERER_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${CLUSTERER_TARGET} INTERFACE ufjfmltk::core)

# Specify installation targets, typology and destination folders.
install(TARGETS ${CLUSTERER_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${CLUSTERER_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/clusterer")

install(FILES include/ufjfmltk/Clusterer.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CLUSTERER_TARGET} for export ${PROJECT_NAME}.")