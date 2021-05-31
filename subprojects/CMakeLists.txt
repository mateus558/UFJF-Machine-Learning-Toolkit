add_subdirectory(ufjfmltk-core)
add_subdirectory(ufjfmltk-classifier)
add_subdirectory(ufjfmltk-valid)
add_subdirectory(ufjfmltk-clusterer)
add_subdirectory(ufjfmltk-ensemble)
add_subdirectory(ufjfmltk-featselect)
add_subdirectory(ufjfmltk-regressor)
add_subdirectory(ufjfmltk-visual)

add_library(${PROJECT_NAME}
        $<TARGET_OBJECTS:ufjfmltk::core>
        $<TARGET_OBJECTS:ufjfmltk::classifier>
        $<TARGET_OBJECTS:ufjfmltk::visual>
        ../include/ufjfmltk/ufjfmltk.hpp
        )

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION  ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "../include/ufjfmltk/ufjfmltk.hpp")

target_link_libraries(${PROJECT_NAME} INTERFACE ufjfmltk::regressor ufjfmltk::ensemble
        ufjfmltk::clusterer ufjfmltk::featselect ufjfmltk::valid ufjfmltk::visual)
