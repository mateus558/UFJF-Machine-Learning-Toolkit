add_library(${LIBVALIDATION} SHARED
    Validation.hpp
)

set_target_properties(${LIBVALIDATION} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${LIBVALIDATION} PROPERTIES PUBLIC_HEADER "Validation.hpp")

target_compile_definitions(${LIBVALIDATION} PUBLIC LIBVALIDATION_VERSION=1.0)
target_compile_features(${LIBVALIDATION} PRIVATE cxx_std_14)
target_link_libraries(${LIBVALIDATION} ${LIBCORE} ${LIBCLASSIFIER})

install_lib(${LIBVALIDATION} Validation 1.0)