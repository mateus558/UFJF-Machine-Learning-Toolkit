# This is the "physical" (i.e. real) name of the library.
# The actual file name of the library built is constructed based on conventions
# of the native platform, such as lib<VALID_TARGET>.a,
# lib<VALID_TARGET>.dylib or <VALID_TARGET>.lib.
set(VALID_TARGET valid)

# List of HPP (header) library files.
set(${VALID_TARGET}_HDR
        include/ufjfmltk/valid/Validation.hpp
        )

# Adds a library target called ${VALID_TARGET} to be built from the
# source and header files listed in the command invocation.
add_library(${VALID_TARGET} INTERFACE)
add_library(${PROJECT_NAME}::${VALID_TARGET} ALIAS ${VALID_TARGET})

# Specify include directories for both compilation and installation process.
target_include_directories(${VALID_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${VALID_TARGET} INTERFACE ufjfmltk::core ufjfmltk::classifier)

# Specify installation targets, typology and destination folders.
install(TARGETS ${VALID_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${VALID_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/valid")

install(FILES include/ufjfmltk/Validation.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${VALID_TARGET} for export ${PROJECT_NAME}.")