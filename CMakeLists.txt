cmake_minimum_required(VERSION 3.5)
if(UNIX)
    set(CMAKE_INSTALL_PREFIX /usr)
endif(UNIX)
project(ufjfmltk
        LANGUAGES CXX C
        VERSION 0.51.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Under MSVC, we set CMAKE_DEBUG_POSTFIX to "d" to add a trailing "d" to library
# built in debug mode. In this Windows user can compile, build and install the
# library in both Release and Debug configuration avoiding naming clashes in the
# installation directories.
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if(CODE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif(CODE_COVERAGE)

### Options
# Shared/Dynamic or Static library?
option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" OFF)

# Build test related commands?
option(ENABLE_TESTING "Create tests using CMake" OFF)

# Tells wheter a module should be compiled
option(BUILD_LIBCLASSIFIER "Tells if the classifier module must be built" ON)
option(BUILD_LIBREGRESSOR "Tells if the regression module must be built" ON)
option(BUILD_LIBVISUALIZE "Tells if the visualization module must be built" ON)
option(BUILD_LIBCLUSTERER "Tells if the clusterer module must be built" ON)
option(BUILD_LIBFEATSELECT "Tells if the feature selection module must be built" ON)
option(BUILD_LIBVALIDATION "Tells if the validation module must be built" ON)

# Enable RPATH support for installed binaries and libraries
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
        LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
        INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
        USE_LINK_PATH)

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

### Compile- and install-related commands.
add_subdirectory(subprojects)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION  ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER include/ufjfmltk/ufjfmltk.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk" COMPONENT dev)

install(FILES include/ufjfmltk/ufjfmltk.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

message(STATUS "Created target ${PROJECT_NAME} for export ${PROJECT_NAME}.")


# Add integration tests (unit tests for each library should be in each sublibrary directory).
if(ENABLE_TESTING)
    message(STATUS "Creating tests for ${PROJECT_NAME}.")

    set(DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/datasets)
    enable_testing()
    add_subdirectory(tests)
endif()

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
        VERSION ${${PROJECT_NAME}_VERSION}
        COMPATIBILITY AnyNewerVersion
        VARS_PREFIX ${PROJECT_NAME}
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)
# Add the uninstall target
include(AddUninstallTarget)


# Add targets related to doxygen documention generation
add_subdirectory(docs)

# add_executable(teste teste.cpp)
# target_link_libraries(teste ${PROJECT_NAME})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(packaging) # include(CPack) will happen in here
endif()

add_executable(teste main.cpp)
target_link_libraries(teste ${PROJECT_NAME})