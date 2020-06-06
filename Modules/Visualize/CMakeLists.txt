find_package(Gnuplot)

if(GNUPLOT_FOUND)
    add_library(${LIBVISUALIZE} SHARED 
        src/gnuplot_i.cpp
        include/gnuplot_i.hpp
        src/Visualization.cpp
    )

    set_target_properties(${LIBVISUALIZE} PROPERTIES PUBLIC_HEADER "include/Visualization.hpp")

    target_include_directories(${LIBVISUALIZE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(${LIBVISUALIZE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_compile_definitions(${LIBVISUALIZE} PUBLIC LIBVISUALIZE_VERSION=1.1)
    target_compile_features(${LIBVISUALIZE} PRIVATE cxx_std_14)
    target_link_libraries(${LIBVISUALIZE} ${LIBCORE})

    include(GNUInstallDirs)

    install(TARGETS ${LIBVISUALIZE}
            LIBRARY DESTINATION
                ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
            PUBLIC_HEADER DESTINATION
                ${CMAKE_INSTALL_INCLUDEDIR}/${LIBVISUALIZE}
            )
else()
    message(WARNING "Gnuplot not found: The Visualization module will not be compiled.")
endif()