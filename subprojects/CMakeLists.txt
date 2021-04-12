add_subdirectory(ufjfmltk-core)
add_subdirectory(ufjfmltk-classifier)
add_subdirectory(ufjfmltk-valid)
add_subdirectory(ufjfmltk-clusterer)
add_subdirectory(ufjfmltk-ensemble)
add_subdirectory(ufjfmltk-featselect)
add_subdirectory(ufjfmltk-regressor)
add_subdirectory(ufjfmltk-visual)
#set(LIBMAIN ${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:ufjfmltk-core>
                                   $<TARGET_OBJECTS:ufjfmltk-classifier>
                                   $<TARGET_OBJECTS:ufjfmltk-clusterer>
                                   $<TARGET_OBJECTS:ufjfmltk-featselect>
                                   $<TARGET_OBJECTS:ufjfmltk-regressor>
                                   $<TARGET_OBJECTS:ufjfmltk-visual>
                                   )
#
install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk" COMPONENT dev)

install(FILES ufjfmltk.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${PROJECT_NAME} for export ${PROJECT_NAME}.")
