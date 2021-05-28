cmake_minimum_required(VERSION 3.5)
if(UNIX)
    set(CMAKE_INSTALL_PREFIX /usr)
endif(UNIX)
project(ufjfmltk
        LANGUAGES CXX C
        VERSION 0.40.0)
set (CMAKE_CXX_STANDARD 17)
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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if(CODE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif(CODE_COVERAGE)

### Options
# Shared/Dynamic or Static library?
option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" ON)

# Build test related commands?
option(ENABLE_TESTING "Create tests using CMake" OFF)

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

# Add integration tests (unit tests for each library should be in each sublibrary directory).
if(ENABLE_TESTING)
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