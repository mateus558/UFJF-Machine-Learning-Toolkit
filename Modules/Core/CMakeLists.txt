add_library(${LIBCORE} SHARED
        src/Data.hpp
        src/Learner.hpp
        src/Point.hpp
        src/Random.hpp
        src/Solution.hpp
        src/Statistics.hpp
        src/Timer.hpp
        src/Utils.hpp
        src/Data.cpp
        src/Statistics.cpp
        src/Utils.cpp
        src/Kernel.hpp
        src/Kernel.cpp
        src/MLToolkit.hpp
)

target_include_directories(${LIBCORE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBCORE} PUBLIC CORE_VERSION=1.1)
target_compile_features(${LIBCORE} PRIVATE cxx_std_14)
