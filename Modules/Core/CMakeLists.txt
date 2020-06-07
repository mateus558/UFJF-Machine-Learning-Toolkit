add_library(${LIBCORE} SHARED
        src/Data.cpp
        src/Statistics.cpp
        src/Utils.cpp
        src/Kernel.cpp
)

set_target_properties(${LIBCORE} PROPERTIES PUBLIC_HEADER "include/Data.hpp;include/Learner.hpp;include/Point.hpp;include/Random.hpp;include/Solution.hpp;include/Statistics.hpp;include/Timer.hpp;include/Utils.hpp;include/MLToolkit.hpp;include/Kernel.hpp")

target_include_directories(${LIBCORE} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBCORE} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBCORE} PUBLIC CORE_VERSION=1.1)
target_compile_features(${LIBCORE} PRIVATE cxx_std_14)

include(GNUInstallDirs)

install(TARGETS ${LIBCORE}
        EXPORT ${PROJECT_NAME}_${LIBCORE}-targets
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${LIBCORE}
        )
install(EXPORT ${PROJECT_NAME}_${LIBCORE}-targets DESTINATION ${CMAKE_INSTALL_DIR}/cmake/${LIBCORE})
