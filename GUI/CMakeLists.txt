find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui DataVisualization Svg Charts)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}")
set(CMAKE_INCLUDE_CURRENT_DIR  ON)

project(${PROJECT_NAME}-gui VERSION 1.0.0)

message(STATUS ${PROJECT_NAME})

set(UI
        ui/barplot2d.ui
        ui/datasetmanager.ui
        ui/mainwindow.ui
        ui/scatterplot2d.ui
        ui/scatterplot3d.ui
        )
set(HEADERS
        include/barplot2d.h
        include/datasetmanager.h
        include/mainwindow.h
        include/scatterplot2d.h
        include/scatterplot3d.h
        include/visualizationbase.h
        )
set(SOURCES
        src/barplot2d.cpp
        src/datasetmanager.cpp
        src/mainwindow.cpp
        src/scatterplot2d.cpp
        src/scatterplot3d.cpp
        src/visualizationbase.cpp
        )

qt5_wrap_ui(HEADERS_WRAPPED ${UI})
qt5_wrap_cpp(SOURCES_MOC ${HEADERS})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS} ${SOURCES_MOC} ${HEADERS_WRAPPED})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ui)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::DataVisualization
        Qt5::Svg
        Qt5::Charts
        ${LIBCORE}
        )

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION
        ${CMAKE_INSTALL_BINDIR}
        )