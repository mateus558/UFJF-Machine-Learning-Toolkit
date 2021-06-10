set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR UFJF)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ machine learning framework.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/mateus558/UFJF-Machine-Learning-Toolkit")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_ICON  "${CMAKE_CURRENT_SOURCE_DIR}/imgs\\\\logo.bmp")
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
if(WIN32)
    set(CPACK_PACKAGING_INSTALL_PREFIX "")
    set(CPACK_NSIS_PACKAGE_NAME "UFJF - Machine Learning Toolkit")
    set(CPACK_NSIS_DISPLAY_NAME "UFJF - Machine Learning Toolkit")
    set(CPACK_NSIS_CONTACT "mateus.marim@ice.ufjf.br")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_GENERATOR ZIP NSIS)

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