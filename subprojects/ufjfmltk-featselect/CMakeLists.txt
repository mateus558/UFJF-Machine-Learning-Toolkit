set(FEATSELECT_TARGET ${PROJECT_NAME}-featselect)

# List of HPP (header) library files.
set(${FEATSELECT_TARGET}_HDR
        include/ufjfmltk/featselect/AOS.hpp
        include/ufjfmltk/featselect/FeatureSelection.hpp
        include/ufjfmltk/featselect/Fisher.hpp
        include/ufjfmltk/featselect/Golub.hpp
        include/ufjfmltk/featselect/RFE.hpp
        include/ufjfmltk/FeatureSelection.hpp
        )

# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${FEATSELECT_TARGET} INTERFACE)
add_library(${PROJECT_NAME}::${FEATSELECT_TARGET} ALIAS ${FEATSELECT_TARGET})

# Specify include directories for both compilation and installation process.
target_include_directories(${FEATSELECT_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${FEATSELECT_TARGET} INTERFACE ufjfmltk-core ufjfmltk-classifier ufjfmltk-valid)

# Specify installation targets, typology and destination folders.
install(TARGETS ${FEATSELECT_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${FEATSELECT_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/featselect")

install(FILES include/ufjfmltk/FeatureSelection.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${FEATSELECT_TARGET} for export ${PROJECT_NAME}.")