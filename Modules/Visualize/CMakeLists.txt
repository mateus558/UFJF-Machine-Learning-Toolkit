find_package(Gnuplot)

if(GNUPLOT_FOUND)
    add_library(${LIBVISUALIZE} SHARED 
        src/gnuplot_i.cpp
        src/gnuplot_i.hpp
        src/Visualization.hpp
        src/Visualization.cpp
    )
    target_include_directories(${LIBVISUALIZE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_compile_definitions(${LIBVISUALIZE} PUBLIC LIBVISUALIZE_VERSION=1.1)
    target_compile_features(${LIBVISUALIZE} PRIVATE cxx_std_14)
    target_link_libraries(${LIBVISUALIZE} ${LIBCORE})
else()
    message(WARNING "Gnuplot not found: The Visualization module will not be compiled.")
endif()