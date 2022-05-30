message("Testing enabled.")
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(REGRESSOR_TESTS regressor_test)
add_subdirectory(regressor)
set(VISUALIZATION_TESTS visualization_test)
add_subdirectory(visualization)
set(CORE_TESTS core_test)
add_subdirectory(core)
set(CLASSIFIER_TESTS classifier_test)
add_subdirectory(classifier)
set(CLUSTERER_TESTS clusterer_test)
add_subdirectory(clusterer)
set(ENSEMBLE_TESTS ensemble_test)
add_subdirectory(ensemble)
set(FEATSELECT_TESTS featselect_test)
add_subdirectory(featselect)

if(CODE_COVERAGE)
    set(COV_TARGETS ${FEATSELECT_TESTS} ${REGRESSOR_TESTS} ${ENSEMBLE_TESTS} ${CLUSTERER_TESTS} ${CLASSIFIER_TESTS} ${CORE_TESTS} ${VISUALIZATION_TESTS})
    setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE ctest
            DEPENDENCIES ${COV_TARGETS} core clusterer classifier ensemble valid visual regressor
            EXCLUDE "cmake*;subprojects/ufjfmltk-classifier/include/ufjfmltk/classifier/hnswlib/*;subprojects/ufjfmltk-visual/src/gnuplot_i.cpp;subprojects/ufjfmltk-visual/include/ufjfmltk/visual/gnuplot_i.hpp"
    )
endif(CODE_COVERAGE)