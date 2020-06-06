set(EXECNAME ufjf-mltk)

add_executable(${EXECNAME} main.cpp)
target_link_libraries(${EXECNAME} PRIVATE ${LIBCORE})