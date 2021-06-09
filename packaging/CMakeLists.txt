set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR UFJF)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ machine learning framework.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)
if(WIN32)
    set(CPACK_PACKAGING_INSTALL_PREFIX "")
    set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ufjfmltk)
    set(CPACK_GENERATOR ZIP WIX IFW)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR RPM DEB IFW)
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mateus Coutinho Marim")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "gnuplot,gnuplot-x11")
else()
    set(CPACK_GENERATOR TGZ)
endif()
set(CPACK_SOURCE_IGNORE_FILES
        /\\.git/
        "/cmake-build-debug/"
        "/cmake-build-release/"
        "/cmake-build-test/"
        "/cmake-build-debug-coverage/"
        "/build/"
        "/builddir/"
        "/debug/"
        "/release/"
        "/wraps/"
        "/.idea/"
        "/.github/"
        "/.vscode/"
        "/docs/"
        "/tests/"
        "/datasets/"
        "../packaging/"
        )

include(CPack)